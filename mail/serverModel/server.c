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
#include <semaphore.h>
#include"sys/queue.h"
#include<stdlib.h>
#define MAX_LINE 100
#define PORT 3339
int s_fd = 0;
//int c_fd = 0;
int rcvThreadCanRun= 1;
sem_t gMainMsgSem;
/** @fn    set_nonblock(int32 sock_fd, bool b_set)
 *  @brief    设置socket fd为阻塞模式或者非阻塞模式
 *  @param[in]  sock_fd 已经连接成功的连接fd。范围:大于0
 *  @param[in]  b_set 是否设置为非阻塞模式。0-否，非0-是；
 *  @param[out] 无
 *  @return 成功返回0；失败返回-1
 */

typedef struct{
    pthread_t thread_id[30];        /* ID returned by pthread_create() */
    int client_fd[30];
    int cur_num;
}ClientList;

ClientList gClientList;

typedef struct{
    int client_fd;
}rcvThreadArg;



struct Msg{
    size_t msgLen;
    int clientFd;
    char  msgData[100];
    TAILQ_ENTRY(Msg) field;
};

TAILQ_HEAD(MsgHead,Msg);

struct MsgHead gMsgHeader;

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
    printf("field s_fd:%d \n", sock_fd);
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

void *recvThread(void *arg)//接收数据线程入口函数。
{
    puts("recvThread start." );
    rcvThreadArg *thArg = (rcvThreadArg *)arg;
    const int bufLen = 200;
    char recvBuff[bufLen];
    printf("thread have send hello msg to client fd:%d \n", thArg->client_fd);
    while (rcvThreadCanRun)
    {
        memset(recvBuff, 0, bufLen);
        int ret = recv(thArg->client_fd, recvBuff, bufLen, 0);
        usleep(5000);
        if (ret == 0)//客户端关闭套接字，则返回0，否则-1.
        {
            rcvThreadCanRun = 0;
            perror("client is disconnection! ");
        }
        else if (ret>0)
        {
            sem_post(&gMainMsgSem);
            struct Msg * msg =(struct Msg *) malloc(sizeof(struct Msg));
            memcpy(msg->msgData, recvBuff, 100);
            msg->clientFd = thArg->client_fd;
            //TAILQ_INSERT_TAIL(head, elm, field)
            TAILQ_INSERT_TAIL(&gMsgHeader,msg,field);
            printf("\nrecv:%s\n ",  recvBuff );
        }
    }
    gClientList.cur_num--;
    puts("recvThread normal exit.");
    return NULL;
}
void* AcceptConnection(void*arg)
{
    printf("wait for connection   ");
    while (1)
    {
        struct sockaddr_in c_addr;
        int addrlen=sizeof(struct sockaddr_in);
        /*int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
         *On success, these system calls return a nonnegative integer 
         *that is a file descriptor for the accepted socket.  
         *On error, -1 is returned, and errno is set appropriately. */
        int c_fd = accept(s_fd, (struct sockaddr*)&c_addr, (socklen_t*__restrict)&addrlen);
        if (c_fd == -1)
        {
            if (errno == EWOULDBLOCK)
            {
                sleep(1);
            }
            else
            {
                perror(__func__);
                continue;
            }
        }
        else//收到客户端请求。
        {
            puts("\nhave already connected.");
            const char *hello= "have establish connection with server.";
            /*On success, these calls return the number of bytes sent.
             * On error, -1 is returned, and errno is set appropriately.*/
            int ret = send(c_fd,hello, strlen(hello), 0);
            if( ret > 0 )
            {
                printf("have send hello msg to client fd:%d \n", c_fd);
                static rcvThreadArg thArg={0};
                thArg.client_fd = c_fd;
                if(0 !=pthread_create(&gClientList.thread_id[gClientList.cur_num], NULL, recvThread, &thArg))
                {
                    perror("pthread_create failed:");
                    return NULL;
                }
                else{
                    gClientList.client_fd[gClientList.cur_num++] = c_fd;
                }
            }
            else{
                perror("send");
            }
        }
    }
    return NULL;
}



int main()
{
    TAILQ_INIT(&gMsgHeader);
    sem_init(&gMainMsgSem, 0, 0);
    struct sockaddr_in s_addr;

    int addrlen=sizeof(s_addr);

    s_fd=socket(AF_INET, SOCK_STREAM,0);
    setNonblock(s_fd, 1);

    s_addr.sin_family =AF_INET;
    s_addr.sin_addr.s_addr=htonl( INADDR_ANY);
    s_addr.sin_port= PORT;

    bind(s_fd,(struct sockaddr*)&s_addr, addrlen);

    /*int listen(int sockfd, int backlog);
     * The backlog argument defines the maximum length to 
     * which the queue of pending connections for sockfd may grow.  */
    listen(s_fd,30);
    pthread_t thread_id; 
    if(0 !=pthread_create(&thread_id, NULL, AcceptConnection, NULL))
    {
        perror("pthread_create AcceptConnection failed:");
        return -2;
    }

    while (1)
    {
        sem_wait(&gMainMsgSem);
        puts("get sem");
        struct Msg *var = TAILQ_FIRST(&gMsgHeader);
        {
            printf("current thNum:%d ,msg from fd:%d ,msg:%s\n",gClientList.cur_num, var->clientFd,var->msgData);
        }
        for( int i = 0 ; i<gClientList.cur_num ; ++i )
        {
            if(send(gClientList.client_fd[i], var->msgData, strlen(var->msgData)+1,0) == -1)
            {
                perror("send failed！");
                break;
            }
            printf("send msg to fd:%d\n", gClientList.client_fd[i]);
            fflush(stdout);

        }

        TAILQ_REMOVE(&gMsgHeader, var, field);
    }
    puts("main exit！");

    close(s_fd);
    return 0;
}

