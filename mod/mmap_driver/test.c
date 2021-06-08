/*********************************************************
    > File Name: test.c
    > Author: ims
    > Created Time: Mon 07 Jun 2021 11:32:28 PM CST
 *********************************************************/


#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <string.h>

int main( void )
{
    int fd;
    char *mapBuf;
    fd = open("/dev/mmap_driver", O_RDWR);//打开设备文件，内核就能获取设备文件的索引节点，填充inode结构
    if(fd<0)
    {
        printf("open device is error,fd = %d\n",fd);
        return -1;
    }
    /*测试一：查看内存映射段*/
    printf("before mmap\n");
    sleep(10);//睡眠10秒，查看映射前的内存图cat /proc/pid/maps
    mapBuf = mmap(NULL, 1024, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);//内存映射，会调用驱动的mmap函数
	printf("kbuf data:%s\n",mapBuf);
    printf("after mmap\n");
    sleep(15);//睡眠15秒，在命令行查看映射后的内存图，如果多出了映射段，说明映射成功

    /*测试二：往映射段读写数据，看是否成功*/
    strcpy(mapBuf, "Driver Test from user" );//向映射段写数据

    munmap(mapBuf, 1024);//去除映射
    close(fd);//关闭文件，最终调用驱动的close
    return 0;
}

