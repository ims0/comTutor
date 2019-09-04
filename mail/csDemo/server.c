#include<sys/types.h>
#include<sys/socket.h>
#include<stdio.h>
#include<string.h>
#include<netinet/in.h>
#include<unistd.h>
#include<arpa/inet.h>
#define MAX_LINE 100
#define PORT 3339 
 
 
int main()
{    
    const char *sendbuf= "this is a server return string ,thanks";
    char buf[256];
    int s_fd,c_fd;
    
    struct sockaddr_in s_addr;    
    struct sockaddr_in c_addr;    
 
    int addrlen=sizeof(s_addr);
 
    s_fd=socket(AF_INET, SOCK_STREAM,0);
    s_addr.sin_family =AF_INET;
    s_addr.sin_addr.s_addr=htonl( INADDR_ANY);
    s_addr.sin_port= PORT;
 
    bind(s_fd,(struct sockaddr*)&s_addr, addrlen );
 
    listen(s_fd,30);
    printf("waiting for connect~~~~\n");    
    /*
     On  success,  these  system  calls  return  a  nonnegative integer that is a descriptor for the accepted socket.  On error, -1 is
     returned, and errno is set appropriately.
    */
    c_fd = accept(s_fd,(struct sockaddr*)&c_addr, (socklen_t*__restrict)&addrlen);
    
    if(-1==c_fd)
    {
        perror("accept failed! because");//print errno num of last error
        return -1;
    }
    else
        puts("accepted!\n");    
 
    const char *startstr="this is server, now we can communicate.";
    send(c_fd,startstr ,strlen(startstr),0);
 
    while(1)
    {    
        printf("recv---\n");
        recv(c_fd, buf, 256,0);
        printf("recv:%s\nsend:", buf );
        if(EOF!=scanf("%s",buf))
        send(c_fd,buf,strlen(buf)+1,0);
        else 
            break;
    }    
    
    
    close(s_fd);
    close(c_fd);
    return 0;
}

