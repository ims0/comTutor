#include<sys/types.h>
#include<sys/socket.h>
#include<stdio.h>
#include<string.h>
#include<netinet/in.h>
#include<unistd.h>
#include<arpa/inet.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>     //for strerror()
#include<pthread.h>
#define MAX_LINE 100
#define PORT 3339
int s_fd = 0;
int c_fd = 0;
int rcvThreadCanRun= 0;
/** @fn    set_nonblock(int32 sock_fd, bool b_set)
 *  @brief    设置socket fd为阻塞模式或者非阻塞模式
 *  @param[in]  sock_fd 已经连接成功的连接fd。范围:大于0
 *  @param[in]  b_set 是否设置为非阻塞模式。0-否，非0-是；
 *  @param[out] 无
 *  @return 成功返回0；失败返回-1
 */
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
int AcceptConnection()
{
    printf("wait for connection   ");
    while (1)
    {
        struct sockaddr_in c_addr;
        int addrlen=sizeof(struct sockaddr_in);
        c_fd = accept(s_fd, (struct sockaddr*)&c_addr, (socklen_t*__restrict)&addrlen);
        if (c_fd == -1)
        {
            if (errno == EWOULDBLOCK)
            {
                printWait();
                sleep(1);
            }
            else
            {
                perror("未知错误，接受客户端连接线程退出。" );
                return -1;
            }
        }
        else//收到客户端请求。
        {
            puts("\nhave already connected.");
            const char *hello= "have establish connection.";
            int ret = send(c_fd,hello, strlen(hello), 0);
            rcvThreadCanRun = 1;
            break;
        }
    }
    return 0;
}
void *recvThread(void *arg)//接收数据线程入口函数。
{
    puts("recvThread start." );
    const int bufLen = 200;
    char recvBuff[bufLen];
    while (rcvThreadCanRun)
    {
        printWait();
        memset(recvBuff, 0, bufLen);
        int ret = recv(c_fd, recvBuff, bufLen, 0);
        usleep(500);
        printf("ret:%d", ret );
        if (ret == 0)//客户端关闭套接字，则返回0，否则-1.
        {
            perror("client is disconnection! ");
            rcvThreadCanRun = 0;
            return NULL;
        }
        else if (ret>0)
        {
            printf("\nrecv:%s\n ",  recvBuff );
        }
    }
    perror("recvThread normal exit.");
    return NULL;
}


int main()
{
    char sendBuff[256];


    struct sockaddr_in s_addr;

    int addrlen=sizeof(s_addr);

    s_fd=socket(AF_INET, SOCK_STREAM,0);
    setNonblock(s_fd, 1);

    s_addr.sin_family =AF_INET;
    s_addr.sin_addr.s_addr=htonl( INADDR_ANY);
    s_addr.sin_port= PORT;

    bind(s_fd,(struct sockaddr*)&s_addr, addrlen);

    listen(s_fd,30);
    /*
       On  success,  these  system  calls  return  a  nonnegative integer that is a descriptor for the accepted socket.  On error, -1 is
       returned, and errno is set appropriately.
       */
    if(AcceptConnection() == -1)
        return -1;
    pthread_t subTid;
    if(0 !=pthread_create(&subTid, NULL, recvThread, NULL))
    {
        perror("pthread_create failed:");
        return -2;
    }


    while (1)
    {
        scanf("%s",sendBuff);
        if (!strcmp(sendBuff, "q")||rcvThreadCanRun == 0)
        {
            rcvThreadCanRun = 0;
            puts("wait thread for exit！");
            pthread_join(subTid,NULL);
            return 0;
        }
        if(send(c_fd, sendBuff, strlen(sendBuff)+1,0) == -1)
        {
            perror("send failed！");
            break;
        }
    }

    close(s_fd);
    close(c_fd);
    return 0;
}

