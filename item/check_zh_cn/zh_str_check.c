#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#define BUFF_LEN 10000
#define HEX_BYTE_EF -17
#define HEX_BYTE_BB -69
#define HEX_BYTE_BF -65

enum ReturnCode {
  SUCEESS = 0,
  ARGC_ERR,
  FILE_NAME_ERR,
  FILE_TYPE_ERR,
  INCLUDE_ZH_ERR,
};
int file_size(const char *filename);
void remove_c_annotation(char *buf, size_t size);
void remove_lua_annotation(char *buf, size_t size);
void remove_python_annotation(char *buf, size_t size);

// c / lua struct define
typedef struct {
  char *single_quote, *double_quote, *line_annotation, *block_annotation;
} AnnotationFlag;

typedef int (*AnnoProcFunc)(char *offset, AnnotationFlag *cFlag);

typedef struct {
  int char_val;
  AnnoProcFunc func;
} FuncMap;

// python struct define

int check_zh_cn(char str[], int len) {
  int i = 0;
  if (len > 3 && str[0] == HEX_BYTE_EF && str[1] == HEX_BYTE_BB &&
      str[2] == HEX_BYTE_BF) {
    i = 3;
  }
  for (; i < len; i++) {
    if (str[i] < 0) {
      if (len - i > 20) {
        str[i + 20] = '\0';
        printf("zh_cn str:%s\n", str + i);
      } else {
        printf("zh_cn str:%s\n", str + i);
      }
      return INCLUDE_ZH_ERR;
    }
  }
  return 0;
}
int remove_anno_and_check(char *buf, int bufSize, FILE *fp,
                          const char *filetype) {
  int read_len = fread(buf, 1, bufSize, fp);
  if (strcmp(filetype, "cpp") == 0)
    remove_c_annotation(buf, read_len);
  else if (strcmp(filetype, "lua") == 0)
    remove_lua_annotation(buf, read_len);
  else if (strcmp(filetype, "python") == 0)
    remove_python_annotation(buf, read_len);
  else
    return FILE_TYPE_ERR;
#ifdef DEBUG
  printf("----- no annotation code -----\n%s\n", buf);
#endif
  return check_zh_cn(buf, read_len);
}

int main(int argc, char *argv[]) {
  if (argc < 3) {
    puts("need a arg:c or cpp file");
    return ARGC_ERR;
  }
  FILE *fp = fopen(argv[1], "r");
  if (fp == NULL) {
    puts("fopen fail");
    return FILE_NAME_ERR;
  }
  int fileSize = file_size(argv[1]);
  int result = SUCEESS;
  if (fileSize < BUFF_LEN) {
    char buf[BUFF_LEN];
    result = remove_anno_and_check(buf, fileSize, fp, argv[2]);
  } else {
    void *mBuf = malloc(fileSize);
    result = remove_anno_and_check((char *)mBuf, fileSize, fp, argv[2]);
    free(mBuf);
  }
  fclose(fp);
  return result;
}
static int slash_is_single(const char *ptr) {
  const char *iter = ptr;
  int cnt = 0;
  while (*iter == '\\') {
    iter--;
    cnt++;
  }
  return cnt % 2;
}

int single_quote_proc(char *offset, AnnotationFlag *cFlag) {
  if (cFlag->double_quote || cFlag->line_annotation ||
      cFlag->block_annotation) {
    return 1;
  }
  size_t len = 0;
  if (cFlag->single_quote == NULL) {
    cFlag->single_quote = offset;
    return 1;
  } else {
    len = offset - cFlag->single_quote;
    if (len == 2 && *(cFlag->single_quote + 1) == '\\') {
      return 1;
    }
    cFlag->single_quote = NULL;
    return 1;
  }
}

int double_quote_proc(char *offset, AnnotationFlag *cFlag) {
  if (cFlag->single_quote || cFlag->line_annotation ||
      cFlag->block_annotation) {
    return 1;
  }
  if (cFlag->double_quote == NULL) {
    cFlag->double_quote = offset;
  } else {
    if (slash_is_single(offset - 1)) {
      return 1;
    }
    cFlag->double_quote = NULL;
  }
  return 1;
}

int c_slash_proc(char *offset, AnnotationFlag *cFlag) {
  if (cFlag->single_quote || cFlag->double_quote || cFlag->line_annotation ||
      cFlag->block_annotation) {
    return 1;
  }
  if (*(offset + 1) == '/') {
    cFlag->line_annotation = offset;
    return 2;
  } else if (*(offset + 1) == '*') {
    cFlag->block_annotation = offset;
    return 2;
  } else {
    return 1;
  }
}

int c_star_proc(char *offset, AnnotationFlag *cFlag) {
  if (cFlag->single_quote || cFlag->double_quote || cFlag->line_annotation ||
      cFlag->block_annotation == NULL || *(offset + 1) != '/') {
    return 1;
  }
  memset(cFlag->block_annotation, ' ', offset + 2 - cFlag->block_annotation);
  cFlag->block_annotation = NULL;
  return 2;
}

int end_proc(char *offset, AnnotationFlag *cFlag) {
  if (cFlag->line_annotation == NULL) {
    return 1;
  }
  char c = *(offset - 1);
  memset(cFlag->line_annotation, ' ',
         (c == '\r' ? offset - 1 : offset) - cFlag->line_annotation);
  cFlag->line_annotation = NULL;
  return 1;
}

FuncMap cFuncMap[] = {{'\'', single_quote_proc},
                      {'\"', double_quote_proc},
                      {'/', c_slash_proc},
                      {'*', c_star_proc},
                      {'\n', end_proc}};

void remove_c_annotation(char *buf, size_t size) {
  char *p = buf, *end = buf + size;
  AnnotationFlag cFlag = {NULL, NULL, NULL, NULL};
  size_t map_size = sizeof(cFuncMap) / sizeof(FuncMap);
  while (p < end) {
    for (size_t i = 0; i < map_size; i++) {
      if (*p == cFuncMap[i].char_val) {
        p += cFuncMap[i].func(p, &cFlag);
      }
    }
  }
  if (cFlag.line_annotation) {
    memset(cFlag.line_annotation, ' ', p - cFlag.line_annotation);
  }
}

int sub_proc(char *offset, AnnotationFlag *flag) {
  if (flag->single_quote || flag->double_quote || flag->line_annotation ||
      flag->block_annotation) {
    return 1;
  }
  if (*(offset + 1) == '-') {
    if (*(offset + 2) == '[' && *(offset + 3) == '[') {
      flag->block_annotation = offset;
      return 4;
    } else {
      flag->line_annotation = offset;
      return 2;
    }
  }
  return 1;
}

int close_bracket_proc(char *offset, AnnotationFlag *flag) {
  if (flag->line_annotation) {
    return 1;
  }
  if (flag->block_annotation && (*(offset + 1) == ']')) {
    memset(flag->block_annotation, ' ', offset + 2 - flag->block_annotation);
    flag->block_annotation = NULL;
    return 2;
  }
  return 1;
}

FuncMap luaFuncMap[] = {{'\'', single_quote_proc},
                        {'\"', double_quote_proc},
                        {'-', sub_proc},
                        {']', close_bracket_proc},
                        {'\n', end_proc}};

void remove_lua_annotation(char *buf, size_t size) {
  char *p = buf, *end = buf + size;
  AnnotationFlag flag = {NULL, NULL, NULL, NULL};
  size_t map_size = sizeof(luaFuncMap) / sizeof(FuncMap);
  int is_comm_char = 1;
  while (p < end) {
    size_t i = 0;
    for (; i < map_size; i++) {
      if (*p == cFuncMap[i].char_val) {
        cFuncMap[i].func(p, &flag);
        is_comm_char = 0;
      }
    }
    if (is_comm_char) {
      p++;
    }
  }
  if (flag.line_annotation) {
    memset(flag.line_annotation, ' ', p - flag.line_annotation);
  }
}

int file_size(const char *filename) {
  struct stat statbuf;
  stat(filename, &statbuf);
  return statbuf.st_size;
}
static int isAssign(char *iter) {
  while (isspace(*iter)) {
    iter--;
  }
  return *iter == '=' ? 1 : 0;
}

void remove_python_annotation(char *buf, size_t size) {
  char *p = buf, *end = buf + size, c;
  char *single_quote = 0, *double_quote = 0, *line_annotation = 0,
       *three_single_quote = 0, *three_double_quote = 0;

  while (p < end) {
    c = *p;
    switch (c) {
    case '\'':
      if (line_annotation || double_quote || three_double_quote) {
        p++;
        continue;
      }
      if (*(p + 1) == '\'' && *(p + 2) == '\'') {
        if (three_single_quote == NULL) {
          three_single_quote = p;
        } else {
          p += 3;
          if (isAssign(three_single_quote - 1)) {
            continue;
          } else {
            memset(three_single_quote, ' ', p - three_single_quote);
            three_single_quote = NULL;
          }
        }
      }
      if (single_quote == NULL) {
        single_quote = p++;
      } else {
        if (*(p++ - 1) == '\\') {
          continue;
        }
        single_quote = NULL;
      }
      break;
    case '\"':
      if (line_annotation || single_quote || three_single_quote) {
        p++;
        continue;
      }
      if (*(p + 1) == '\"' && *(p + 2) == '\"') {
        if (three_double_quote == NULL) {
          three_double_quote = p;
        } else {
          p += 3;
          if (isAssign(three_double_quote - 1)) {
            continue;
          } else {
            memset(three_double_quote, ' ', p - three_double_quote);
            three_double_quote = NULL;
          }
        }
      }
      if (double_quote == NULL) {
        double_quote = p++;
      } else {
        if (*(p++ - 1) == '\\') {
          continue;
        }
        double_quote = NULL;
      }
      break;
    case '#':
      if (single_quote || double_quote || line_annotation ||
          three_single_quote || three_double_quote) {
        p++;
        continue;
      }
      line_annotation = p;
      break;
    case '\n':
      if (line_annotation == NULL) {
        p++;
        continue;
      }
      c = *(p - 1);
      memset(line_annotation, ' ',
             (c == '\r' ? (p++ - 1) : p++) - line_annotation);
      line_annotation = NULL;
      break;
    default:
      p++;
      break;
    }
  }
  if (line_annotation) {
    memset(line_annotation, ' ', p - line_annotation);
  }
}
