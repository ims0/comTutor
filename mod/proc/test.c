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


    return 0;

}

