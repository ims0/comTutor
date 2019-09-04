#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#define MAX_LINE 100
#define PORT 3339 
 
int main(int argc , char*argv[])
{
    int    sockfd= socket(AF_INET, SOCK_STREAM,0);
    
    struct sockaddr_in addr;
    int len_addr=sizeof(addr);
 
    bzero(&addr,len_addr);
    addr.sin_family=AF_INET;
    inet_pton(AF_INET,"127.0.0.1", &addr.sin_addr);
//    addr.sin_addr.s_addr= htonl(INADDR_ANY);
    addr.sin_port= PORT;
    int con= connect(sockfd, (struct sockaddr*)&addr, len_addr);
 
    if(-1==con)
    {
        perror("connect failed! because");//print errno num of last error
        return -1;
    }
    else
        puts("connected!");
 
    char buf[256];
    recv(sockfd, buf, 256,0);
    printf("recv:%s\nsend:", buf );
 
    while(EOF!=scanf( "%s",buf))
    {    
        send(sockfd,buf,strlen(buf)+1,0);
        recv(sockfd, buf, 256,0);
        printf("recv:%s\nsend:", buf );
    }
    close(sockfd);
    return 0;
 
}

