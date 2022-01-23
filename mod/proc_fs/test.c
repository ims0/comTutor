/*********************************************************
    > File Name: app.c
    > Author: ims
    > Created Time: Sat 15 May 2021 10:15:54 PM CST
 *********************************************************/
#include "const.h"

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main(void) {
  char buf[BUFF_SIZE];
  int fd = open(PROC_FILE, O_RDWR);
  if (fd < 0) {
    printf("open:%s failed!\n", PROC_FILE);
    return fd;
  }
  printf("fd:%d\n", fd);

  read(fd, buf, BUFF_SIZE);
  printf("%s\n", buf);

  lseek(fd, 0, SEEK_SET); // The file offset is set to offset bytes
  write(fd, "33 4", 5);

  lseek(fd, 0, SEEK_SET);
  read(fd, buf, BUFF_SIZE);
  puts(buf);

  return 0;
}
