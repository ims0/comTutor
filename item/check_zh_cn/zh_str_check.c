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
  printf("-----code-----\n%s\n", buf);
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
void remove_c_annotation(char *buf, size_t size) {
  char *p = buf, *end = buf + size, c;
  char *single_quote = 0, *double_quote = 0, *line_annotation = 0,
       *block_annotation = 0;
  size_t len = 0;

  while (p < end) {
    c = *p;
    switch (c) {
    case '\'':
      if (double_quote || line_annotation || block_annotation) {
        p++;
        continue;
      }
      if (single_quote == NULL) {
        single_quote = p++;
      } else {
        len = p++ - single_quote;
        if (len == 2 && *(single_quote + 1) == '\\') {
          continue;
        }
        single_quote = NULL;
      }
      break;
    case '\"':
      if (single_quote || line_annotation || block_annotation) {
        p++;
        continue;
      }
      if (double_quote == NULL) {
        double_quote = p++;
      } else {
        if (slash_is_single(p - 1)) {
          p++;
          continue;
        }
        double_quote = NULL;
      }
      break;
    case '/':
      if (single_quote || double_quote || line_annotation || block_annotation) {
        p++;
        continue;
      }
      c = *(p + 1);
      if (c == '/') {
        line_annotation = p;
        p += 2;
      } else if (c == '*') {
        block_annotation = p;
        p += 2;
      } else {
        p++;
      }
      break;
    case '*':
      if (single_quote || double_quote || line_annotation ||
          block_annotation == NULL) {
        p++;
        continue;
      }
      if (*(p + 1) != '/') {
        p++;
        continue;
      }
      p += 2;
      memset(block_annotation, ' ', p - block_annotation);
      block_annotation = NULL;
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
void remove_lua_annotation(char *buf, size_t size) {
  char *p = buf, *end = buf + size, c;
  char *single_quote = 0, *double_quote = 0, *line_annotation = 0,
       *block_annotation = 0;
  size_t len = 0;

  while (p < end) {
    c = *p;
    switch (c) {
    case '\'':
      if (double_quote || line_annotation || block_annotation) {
        p++;
        continue;
      }
      if (single_quote == NULL) {
        single_quote = p++;
      } else {
        len = p++ - single_quote;
        if (len == 2 && *(single_quote + 1) == '\\') {
          continue;
        }
        single_quote = NULL;
      }
      break;
    case '\"':
      if (single_quote || line_annotation || block_annotation) {
        p++;
        continue;
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
    case '-':
      if (single_quote || double_quote || line_annotation || block_annotation) {
        p++;
        continue;
      }
      if (*(p + 1) == '-') {
        if (*(p + 2) == '[' && *(p + 3) == '[') {
          block_annotation = p;
          p += 4;
        } else {
          line_annotation = p;
          p = p + 2;
        }
      } else {
        p++;
      }
      break;
    case ']':
      if (line_annotation) {
        p++;
        continue;
      }
      if (block_annotation && (*(p + 1) == ']')) {
        p += 2;
        memset(block_annotation, ' ', p - block_annotation);
        block_annotation = NULL;
      } else {
        p++;
      }
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
