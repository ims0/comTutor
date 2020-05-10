#include<stdio.h>
#include<strings.h>
#include<string.h>
#include<sys/types.h>
#include<arpa/inet.h>

int getSocketFd(int port, const char *ipAddress)
{
    printf("connect fd:%d,ip:%s\n",port,ipAddress);
    int    socketFd= socket(AF_INET, SOCK_STREAM,0);
    struct sockaddr_in socketAddrIn;
    bzero(&socketAddrIn, sizeof(socketAddrIn));
    socketAddrIn.sin_family=AF_INET;
    socketAddrIn.sin_port = htons(port);
    /*inet_pton(int af, const char *src, void *dst);*/
    if(1 != inet_pton(AF_INET, ipAddress, &socketAddrIn.sin_addr))
    {
        perror("inet_pton failed! because:");
        return -1;
    }
    size_t sockAddrLen=sizeof(socketAddrIn);
    if(-1 == connect(socketFd, (struct sockaddr*)&socketAddrIn, sockAddrLen))
    {
        perror("connect failed! because:");
        return -1;
    }
    printf("connect succ return fd:%d\n",socketFd);
    return socketFd;
}
typedef struct tagPSMsg
{
    WORD32 version;
    WORD16 msgId;
    WORD16 priority;
    CHAR*sessionType;
    CHAR*sessionInst;
}PSMsg;

int op_pub(PSMsg *psBrief, BYTE *data, WORD32 dataLen)
{
}
WORD32 pno    = OSS_GetSelfPNO();
int  op_sub(const CHAR* sessionType, const CHAR* sessionInst, WORD32  pno)
{
}
