#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#define BUFF_LEN 5000

int file_size(const char *filename);
void remove_c_annotation(char *buf, size_t size);
void remove_lua_annotation(char *buf, size_t size);

int check_zh_cn(char str[], int len) {
  for (int i = 0; i < len; i++) {
    if (str[i] < 0) {
      if (len - i > 20) {
        str[i + 20] = '\0';
        printf("zh_cn str:%s\n", str + i);
      }
      else{
        printf("zh_cn str:%s\n", str + i);
      }
      return 1;
    }
  }
  return 0;
}

int remove_annotation(char *buf, int bufSize, FILE *fp, int argc) {
  int read_len = fread(buf, 1, bufSize, fp);
  if(argc == 2)
    remove_c_annotation(buf, read_len);
  else
    remove_lua_annotation(buf, read_len);

  printf("-----code-----\n%s\n", buf);
  return check_zh_cn(buf, read_len);
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    puts("need a arg:c or cpp file");
    return 0;
  }
  FILE *fp = fopen(argv[1], "r");
  if (fp == NULL) {
    puts("fopen fail");
    return -1;
  }
  int fileSize = file_size(argv[1]);
  printf("BUFF_LEN:%d,file byte size:%d \n", BUFF_LEN, fileSize);
  int result = 0;
  if (fileSize < BUFF_LEN) {
    char buf[BUFF_LEN];
    result = remove_annotation(buf, fileSize, fp, argc);
  } else {
    void *mBuf = malloc(fileSize);
    result = remove_annotation((char *)mBuf, fileSize, fp, argc);
    free(mBuf);
  }
  fclose(fp);
  return result;
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
        if (double_quote || line_annotation ||
            block_annotation) //在双引号或者注释区域的单引号直接跳过
        {
          p++;
          continue;
        }
        if (single_quote == NULL) //第一个单引号，记录起点
        {
          single_quote = p++;
        } else { //语句中的第二个单引号，
          len = p++ - single_quote;
          if (len == 2 && *(single_quote + 1) == '\\')
            //遇到'\'~~的情况
          {
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
          if (*(p++ - 1) == '\\') //忽略反斜杠后面的双引号
          {
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
        if (c == '/') //遇到双斜杠，行注释
        {
          line_annotation = p;
          p += 2;
        } else if (c == '*') { //块注释
          block_annotation = p;
          p += 2;
        } else { //除号，跳过
          p++;
        }
        break;
      case '*':
        if (single_quote || double_quote || line_annotation ||
            block_annotation == NULL) {
          p++;
          continue;
        }
        if (*(p + 1) != '/') //不是块注释结尾
        {
          p++;
          continue;
        }
        p += 2;
        memset(block_annotation, ' ', p - block_annotation); //块注释的结尾
        block_annotation = NULL;
        break;
      case '\n':
        if (line_annotation == NULL) {
          p++;
          continue;
        }
        c = *(p - 1); //行注释的结尾
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
        if (double_quote || line_annotation ||
            block_annotation) //在双引号或者注释区域的单引号直接跳过
        {
          p++;
          continue;
        }
        if (single_quote == NULL) //第一个单引号，记录起点
        {
          single_quote = p++;
        } else { //语句中的第二个单引号，
          len = p++ - single_quote;
          if (len == 2 && *(single_quote + 1) == '\\')
            //遇到'\'~~的情况
          {
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
          if (*(p++ - 1) == '\\') //忽略反斜杠后面的双引号
          {
            continue;
          }
          double_quote = NULL;
        }
        break;
      case '-':
        if (single_quote || double_quote || line_annotation) {
          p++;
          continue;
        }
        c = *(p + 1);
        if (c == '-')
        {
          if( *(p + 2) == '[' && *(p + 3) == '[' )
          {
            block_annotation = p;
            p += 4;
          }
          else if( block_annotation && *(p + 2) == ']' && *(p + 3) == ']' )
          {
            p += 4;
            memset(block_annotation, ' ', p - block_annotation);
            block_annotation = NULL;
          }
          else{
            line_annotation = p;
            p += 2;
          }
        } else { 
          p++;
        }
        break;
      case ']':
        if (line_annotation ) {
          p++;
          continue;
        }
        if( block_annotation && (*(p + 1) == ']'))
        {
           p += 2;
           memset(block_annotation, ' ', p - block_annotation);
           block_annotation = NULL;
        } else{
            p++;
        }
        break;
      case '\n':
        if (line_annotation == NULL) {
          p++;
          continue;
        }
        c = *(p - 1); //行注释的结尾
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
