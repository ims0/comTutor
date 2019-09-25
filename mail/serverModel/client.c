#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include <fcntl.h>
#include <errno.h>
#include<pthread.h>
#define MAX_LINE 100
#define PORT 3339

#define IP_ADDR  "127.0.0.1"

int rcvThreadCanRun= 1;
int clientSocket = 0;

int setNonblock(int sock_fd, int b_set)
{
    printf("entry s_fd:%d \n", sock_fd);
    int flags = 0;

    flags = fcntl(sock_fd, F_GETFL, 0);
    if (-1 == flags)
    {
        printf("get sockfd flag -1, errno=%s\n", strerror(errno));
        return -1;
    }

    if (b_set)
    {
        if (fcntl(sock_fd, F_SETFL, flags | O_NONBLOCK) == -1)
        {
            printf("set sockfd nonblock -1, errno=%s\n", strerror(errno));
            return -1;
        }
        else
        {
            puts("set nonblock succ!");
        }
    }
    else
    {
        if (fcntl(sock_fd, F_SETFL, flags & (~O_NONBLOCK)) == -1)
        {
            printf("set sockfd block -1, errno=%s\n", strerror(errno));
            return -1;
        }
    }

    return 0;
}
int InitSocket()
{
    /*int socket(int domain, int type, int protocol);
     *On success, a file descriptor for the new socket is returned.
     * On error, -1 is returned, and errno is set appropriately*/
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if(clientSocket == -1)
    {
        perror("socket:");
        return -1;
    }

    setNonblock(clientSocket, 1);

    struct sockaddr_in addr;
    bzero(&addr,sizeof(addr));
    addr.sin_family=AF_INET;

    /*inet_pton - convert IPv4 and IPv6 addresses from text to binary*/
    inet_pton(AF_INET,IP_ADDR, &addr.sin_addr);
    addr.sin_port= PORT;
    /*int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
     *If the connection or binding succeeds, zero is returned.
     * On error, -1 is returned, and errno is set appropriately.*/
    int con= connect(clientSocket, (struct sockaddr*)&addr, sizeof(addr));

    if (con == -1 )
    {
        perror("connect:");
        return -1;
    }
    else
    {
        rcvThreadCanRun = 1;
        puts("connect request succ.");
    }
    return 0;
}
void printWait()
{
    static int cnt=0;
    switch (cnt++)
    {
        case 0:printf("\b\\");break; 
        case 1:printf("\b|");break; 
        case 2:printf("\b/");break; 
        case 3:printf("\b—");break; 
    }
    fflush(stdout);
    if(cnt == 4)
        cnt=0;
}
void *recvThread(void *arg)//接收数据线程入口函数。
{
    puts("recvThread start." );
    const int bufLen = 200;
    char recvBuff[bufLen];
    while (rcvThreadCanRun)
    {
        //printWait();
        memset(recvBuff, 0, bufLen);
        int ret = recv(clientSocket, recvBuff, bufLen, 0);
        usleep(1000);
        if (ret == 0)//客户端关闭套接字，则返回0，否则-1.
        {
            perror("Server is disconnection,input any to exit! ");
            rcvThreadCanRun = 0;
            return NULL;
        }
        else if (ret>0)
        {
            printf("\n%s\n ",  recvBuff );
        }
    }
    puts("recvThread normal exit.");
    return NULL;
}
int main(int argc , char*argv[])
{
    InitSocket();  //发送connect 请求一次就返回，开始启动循环接收进程
    pthread_t subTid;
    if(0 !=pthread_create(&subTid, NULL, recvThread, NULL))
    {
        perror("pthread_create failed:");
        return -2;
    }
    char sendBuff[256];
    while (rcvThreadCanRun)
    {
        scanf("%s",sendBuff);
        if (!strcmp(sendBuff, "q"))
        {
            rcvThreadCanRun = 0;
            puts("wait thread for exit！");
            pthread_join(subTid,NULL);
        }
        if(send(clientSocket, sendBuff, strlen(sendBuff)+1,0) == -1)
        {
            perror("send failed！");
            break;
        }
    }

    close(clientSocket);
    return 0;
}




