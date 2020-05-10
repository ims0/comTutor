#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#define MAX_LINE 100
#define PORT 3002
 
int main(int argc , char*argv[])
{
    int    socketFd= socket(AF_INET, SOCK_STREAM,0);
    
    struct sockaddr_in addr;
    bzero(&addr,sizeof(addr));
    int len_addr=sizeof(addr);
    addr.sin_family=AF_INET;
    inet_pton(AF_INET,"127.0.0.1", &addr.sin_addr);
//    addr.sin_addr.s_addr= htonl(INADDR_ANY);
    addr.sin_port= htons(PORT);
    int con= connect(socketFd, (struct sockaddr*)&addr, len_addr);
 
    if(-1==con)
    {
        perror("connect failed! because");//print errno num of last error
        return -1;
    }
    else
        puts("connected!");
 
    char buf[256];
    recv(socketFd, buf, 256,0);
    printf("recv:%s\nsend:", buf );
 
    while(EOF!=scanf( "%s",buf))
    {    
        send(socketFd,buf,strlen(buf)+1,0);
        recv(socketFd, buf, 256,0);
        printf("recv:%s\nsend:", buf );
    }
    close(socketFd);
    return 0;
 
}

