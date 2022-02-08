/*********************************************************
  > File Name: app.c
  > Author: ims
  > Created Time: Sat 15 May 2021 10:15:54 PM CST
 *********************************************************/

#include "myioctl.h"
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main(void) {
  int ret = 0;
  char file_path[100];
  sprintf(file_path, "%s%s", "/dev/", misc_dev_name);
  int dev_fd = open(file_path, O_RDWR);
  if (dev_fd < 0) {
    printf("open %s fail:%s\n", file_path, strerror(errno));
    return -1;
  }
  printf("open fd:%d\n", dev_fd);

  struct miscdata data;
  data.val = 18;
  data.str = "fellow platform device";
  data.size = sizeof("fellow platform device");
  if ((ret = ioctl(dev_fd, FELLOW_MISC_IOC_SET, &data)) < 0) {
    printf("ioctl set fail:%s\n", strerror(errno));
  }

  struct miscdata getdata;
  if ((ret = ioctl(dev_fd, FELLOW_MISC_IOC_GET, &getdata)) < 0) {
    printf("ioctl get fail:%s\n", strerror(errno));
  }
  printf("ioctl get data:val:%d, str:%s, size: %d\n", getdata.val, getdata.str,
         getdata.size);
  if ((ret = ioctl(dev_fd, FELLOW_MISC_IOC_PRINT, NULL)) < 0) {
    printf("ioctl print fail:%s\n", strerror(errno));
  }
  close(dev_fd);
  return ret;
}

//原文链接：https://blog.csdn.net/wjlinux21/article/details/111691691
