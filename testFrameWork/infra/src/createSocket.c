#include<stdio.h>
#include<string.h>
#include<sys/types.h>
#include<arpa/inet.h>
#define PORT (3001) 
#define IP_ADDR ("127.0.0.1")

int getSocketFd()
{
    int    socketFd= socket(AF_INET, SOCK_STREAM,0);
    struct sockaddr_in addr;
    addr.sin_family=AF_INET;
    addr.sin_port = PORT;
    size_t addrLen=sizeof(addr);
    bzero(&addr, sizeof(addr));
    inet_pton(AF_INET, IP_ADDR, &addr.sin_addr);
    // addr.sin_addr.s_addr= htonl(INADDR_ANY);
    int conn = connect(socketFd, (struct sockaddr*)&addr, addrLen);

    if(-1 == conn)
    {
        perror("connect failed! because");
        return -1;
    }
    return socketFd;
}
