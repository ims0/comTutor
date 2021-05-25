/*********************************************************
    > File Name: app.c
    > Author: ims
    > Created Time: Sat 15 May 2021 10:15:54 PM CST
 *********************************************************/

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/ioctl.h>
#include "myioctl.h"

const char*file_path = "/proc/mydev";

int main(void)
{
    char buf[100];
    int fd = open(file_path, O_RDWR);
    if(fd <0){
        printf("open:%s fail\n", file_path);
        return fd;
    }
    printf("fd:%d\n", fd);

    read(fd, buf, 100);
    printf("%s\n", buf);

    lseek(fd, 0 , SEEK_SET);//The file offset is set to offset bytes
    write(fd, "33 4", 5);

    lseek(fd, 0 , SEEK_SET);
    read(fd, buf, 100);
    puts(buf);


    int dev_fd = open("/dev/fellowplat", O_RDWR);
    if (dev_fd < 0)
    {
        printf("open fail:%s\n", strerror(errno));
        return -1;
    }
    int ret = 0;
    struct miscdata data;
    data.val = 18;
    data.str = "fellow platform device";
    data.size = sizeof("fellow platform device");
    if ((ret = ioctl(dev_fd, FELLOW_MISC_IOC_SET, &data)) < 0)
    {
     printf("ioctl set fail:%s\n", strerror(errno));
    }
    struct miscdata getdata;
    if ((ret = ioctl(dev_fd, FELLOW_MISC_IOC_GET, &getdata)) < 0)
    {
      printf("ioctl get fail:%s\n", strerror(errno));
    }
    printf("get val:%d, str:%s, size: %d\n", getdata.val, getdata.str, getdata.size);
    if ((ret = ioctl(dev_fd, FELLOW_MISC_IOC_PRINT, NULL)) < 0)
    {
      printf("ioctl print fail:%s\n", strerror(errno));
    }
    close(dev_fd);
    return ret;

}

//原文链接：https://blog.csdn.net/wjlinux21/article/details/111691691
