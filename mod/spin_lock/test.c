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
int main()
{
    int fd;
    fd = open("/dev/test",O_RDWR);
    if(fd<0)
    {
        perror("open fail \n");
        return 1;
    }
    sleep(20);
    close(fd);
    printf("open ok \n ");
    return 0;
}

