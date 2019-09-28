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
#define  rcvBufLen  200

int s_fd = 0;
sem_t gMainMsgSem;
pthread_mutex_t gMsgQueueMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t gClientListMutex = PTHREAD_MUTEX_INITIALIZER;


struct ClientInfo{
    pthread_t threadId;        /* ID returned by pthread_create() */
    int clientFd;
    char ip[20];
    TAILQ_ENTRY(ClientInfo) field;
};
typedef struct ClientInfo ClientInfo;
TAILQ_HEAD(ClientHeader,ClientInfo);
struct Client{
    struct ClientHeader clientHead;
    int num;
}gClientList;

typedef struct{
    int client_fd;
    char ip[20];
}rcvThreadArg;



struct Msg{
    size_t msgLen;
    int clientFd;
    char ip[20];
    char  msgData[rcvBufLen];
    TAILQ_ENTRY(Msg) field;
};

TAILQ_HEAD(MsgHead,Msg);

struct MsgHead gMsgQueue;


void dataInit()
{
    TAILQ_INIT(&gMsgQueue);
    TAILQ_INIT(&gClientList.clientHead);
    gClientList.num=0;
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
/** @fn    set_nonblock(int32 sock_fd, bool b_set)
 *  @brief    设置socket fd为阻塞模式或者非阻塞模式
 *  @param[in]  sock_fd 已经连接成功的连接fd。范围:大于0
 *  @param[in]  b_set 是否设置为非阻塞模式。0-否，非0-是；
 *  @param[out] 无
 *  @return 成功返回0；失败返回-1
 */
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
    rcvThreadArg *thArg = (rcvThreadArg *)arg;
    printf("thread have send hello msg to client fd:%d \n", thArg->client_fd);
    while (1)
    {
        struct Msg * msg =(struct Msg *) malloc(sizeof(struct Msg));
        int ret = recv(thArg->client_fd, msg->msgData, rcvBufLen, 0);
        usleep(5000);
        if (ret == 0)//客户端关闭套接字，则返回0，否则-1.
        {
            perror("client is disconnection! ");
            break;
        }
        else if (ret>0)
        {
            msg->clientFd = thArg->client_fd;
            strncpy(msg->ip, thArg->ip,20);
            //TAILQ_INSERT_TAIL(head, elm, field)
            pthread_mutex_lock(&gMsgQueueMutex);
            TAILQ_INSERT_TAIL(&gMsgQueue,msg,field);
            pthread_mutex_unlock(&gMsgQueueMutex);
            sem_post(&gMainMsgSem);
            printf("\nrecv:%s\n",  msg->msgData);
        }
    }
    ClientInfo *client=NULL;
    TAILQ_FOREACH(client, &gClientList.clientHead, field)
    {
        if(client->clientFd == thArg->client_fd)
        {
            pthread_mutex_lock(&gClientListMutex);
            TAILQ_REMOVE(&gClientList.clientHead, client, field);
            gClientList.num--;
            pthread_mutex_unlock(&gClientListMutex);
            break;
        }
    }
    close(thArg->client_fd);
    free(arg);
    free(client);
    puts("recvThread normal exit.");
    return NULL;
}
void* AcceptConnection(void*arg)
{
    printf("wait for connection   ");
    while (1)
    {
        struct sockaddr_in client_addr;
        int addrlen=sizeof(struct sockaddr_in);
        /*int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
         *On success, these system calls return a nonnegative integer 
         *that is a file descriptor for the accepted socket.  
         *On error, -1 is returned, and errno is set appropriately. */
        int c_fd = accept(s_fd, (struct sockaddr*)&client_addr, (socklen_t*__restrict)&addrlen);
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
            printf("ip=%s\n",inet_ntoa(client_addr.sin_addr));
            printf("port=%d\n",ntohs(client_addr.sin_port));
            const char *hello= "have establish connection with server.";
            /*On success, these calls return the number of bytes sent.
             * On error, -1 is returned, and errno is set appropriately.*/
            int ret = send(c_fd,hello, strlen(hello), 0);
            if( ret > 0 )
            {
                printf("have send hello msg to client fd:%d \n", c_fd);
                rcvThreadArg *thArg= (rcvThreadArg *)malloc(sizeof(rcvThreadArg));
                thArg->client_fd = c_fd;
                strncpy(thArg->ip, inet_ntoa(client_addr.sin_addr), 20);
                ClientInfo*client = (ClientInfo*)malloc(sizeof(ClientInfo));
                if(0 !=pthread_create(&client->threadId, NULL, recvThread, thArg))
                {
                    perror("pthread_create failed:");
                    return NULL;
                }
                else{
                    client->clientFd = c_fd;
                    strncpy(client->ip, inet_ntoa(client_addr.sin_addr), 20);

                    pthread_mutex_lock(&gClientListMutex);
                    TAILQ_INSERT_TAIL(&gClientList.clientHead,client,field);
                    gClientList.num++;
                    pthread_mutex_unlock(&gClientListMutex);
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
    dataInit();
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

    char temMsg[200];
    while (1)
    {
        sem_wait(&gMainMsgSem);
        struct Msg *msg = TAILQ_FIRST(&gMsgQueue);
        printf("current thNum:%d ,msg from fd:%d, ip:%s, msg:%s\n",gClientList.num, msg->clientFd,msg->ip, msg->msgData);
        ClientInfo *client=NULL;
        TAILQ_FOREACH(client, &gClientList.clientHead, field)
        {
            if(client->clientFd != msg->clientFd)
            {
                memset(temMsg, 0, sizeof(temMsg));
                strcpy(temMsg,msg->ip);
                if(send(client->clientFd, strcat(strcat(temMsg, ":"),msg->msgData),strlen(msg->msgData)+ strlen(temMsg)+1,0) == -1)
                {
                    perror("send failed！");
                    break;
                }
                else{
                    printf("send msg to fd:%d\n", client->clientFd);
                }
            }
            else{
                if(send(client->clientFd, "have send.", strlen("have send."),0) == -1)
                {
                    perror("send failed！");
                    break;
                }
            }
        }
        pthread_mutex_lock(&gMsgQueueMutex);
        TAILQ_REMOVE(&gMsgQueue, msg, field);
        pthread_mutex_unlock(&gMsgQueueMutex);
        free(msg);
    }
    puts("main exit！");

    close(s_fd);
    return 0;
}

