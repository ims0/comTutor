/*********************************************************
  > File Name: main.cpp
  > Author: ims
  > Created Time: Tue 08 Jun 2021 11:39:30 PM CST
 *********************************************************/
/* use iptables filter ping packet from other host
 * sudo iptables -A INPUT -p icmp --icmp-type echo-request -j DROP
 * sudo iptables -L INPUT --line-numbers
 * sudo iptables -D INPUT <rule line> 
 */

/* icmp protocol 
 * https://datatracker.ietf.org/doc/html/rfc792
 */
//gcc default opt is gun11, if opt set to std=c11, need add macro _GNU_SOURCE
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <unistd.h>
#include <signal.h>
#include <arpa/inet.h>
#include <errno.h>
#include <sys/time.h>
#include <string.h>
#include <netdb.h>
#include <pthread.h>         

#define PACKET_SEND_MAX_NUM 128
#define ICMP_PACKET_HDR_LEN 8
#define ICMP_PACKET_PACKET_LEN (ICMP_PACKET_HDR_LEN + sizeof(struct timeval))


int alive;
int pf_saw_icmp_socket;
int send_count;
int recv_count;
struct timeval start_time;

void print_dest_ip(in_addr_t dest_addr,const char*ip_or_domain)
{
    printf("PING %s, (%d.%d.%d.%d) 56(84) bytes of data.\n",ip_or_domain,
            (dest_addr&0x000000ff), (dest_addr&0x0000ff00)>>8,
            (dest_addr&0x00ff0000)>>16, (dest_addr&0xff000000)>>24);
}
void print_recv_ip(in_addr_t dest_addr, size_t len)
{
    printf("recv PING (%d.%d.%d.%d) %zu bytes of data.\n",
            (dest_addr&0x000000ff), (dest_addr&0x0000ff00)>>8,
            (dest_addr&0x00ff0000)>>16, (dest_addr&0xff000000)>>24, len);
}
/*校验和算法*/
unsigned short cal_chksum(unsigned short *addr,int len)
{       
    int nleft=len;
    int sum=0;
    unsigned short *w=addr;
    unsigned short answer=0;

    /*把ICMP报头二进制数据以2字节为单位累加起来*/
    while(nleft>1)
    {
        sum+=*w++;
        nleft-=2;
    }
    /*若ICMP报头为奇数个字节，会剩下最后一字节。把最后一个字节视为一个2字节数据的高字节，这个2字节数据的低字节为0，继续累加*/
    if( nleft==1)
    {
        *(unsigned char *)(&answer)=*(unsigned char *)w;
        sum+=answer;
    }
    sum=(sum>>16)+(sum&0xffff);
    sum+=(sum>>16);
    answer=~sum;
    return answer;
}

long cal_time_offset(struct timeval begin, struct timeval end)
{
    struct timeval ans;
    ans.tv_sec = end.tv_sec - begin.tv_sec;
    ans.tv_usec = end.tv_usec - begin.tv_usec;
    if(ans.tv_usec < 0) //如果接收时间的usec小于发送时间的usec，则向sec域借位
    {
        ans.tv_sec--;
        ans.tv_usec+=1000000;
    }
    return ans.tv_sec*1000 + ans.tv_usec/1000; //毫秒为单位
}

/*类型|代码|校验和
 *  标识符 |序号
 * 可选数据
 */
void icmp_pack(struct icmp* icmphdr, int seq, int length)
{

    icmphdr->icmp_type = ICMP_ECHO;
    icmphdr->icmp_code = 0;
    icmphdr->icmp_cksum = 0;

    icmphdr->icmp_id = getpid() & 0xffff;
    icmphdr->icmp_seq = seq;

    /*
       for(i=0;i<length;i++)
       {
       icmphdr->icmp_data[i] = i;
       }
       */

    gettimeofday((struct timeval *)&(icmphdr->icmp_data[0]), NULL);
    icmphdr->icmp_cksum = cal_chksum((unsigned short*)icmphdr, length);
}

int icmp_unpack(char* buf, int len)
{
    struct ip* ip_hdr = (struct ip *)buf;
    int iphdr_len = ip_hdr->ip_hl*4;
    int icmp_len = len - iphdr_len;  //icmp包长度
    if(icmp_len < ICMP_PACKET_HDR_LEN)   //判断长度是否为ICMP包长度
    {
        fprintf(stderr, "Invalid icmp packet.Its length is less than %d\n", ICMP_PACKET_HDR_LEN);
        return -1;
    }

    struct icmp* icmp = (struct icmp*)(buf+iphdr_len);
    //判断该包是ICMP回送回答包且该包是我们发出去的
    if((icmp->icmp_type == ICMP_ECHOREPLY) && (icmp->icmp_id == (getpid() & 0xffff)))
    {
        if((icmp->icmp_seq < 0) || (icmp->icmp_seq > PACKET_SEND_MAX_NUM))
        {
            fprintf(stderr, "icmp packet seq is out of range!\n");
            alive = 0;
            return -1;
        }

        struct timeval recv_time;
        gettimeofday(&recv_time, NULL);

        struct timeval *send_time = (struct timeval*)(&icmp->icmp_data[0] );
        long rtt = cal_time_offset(*send_time, recv_time);

        printf("%d byte from %s: icmp_seq=%u ttl=%d rtt=%ld ms\n",
                icmp_len, inet_ntoa(ip_hdr->ip_src), icmp->icmp_seq, ip_hdr->ip_ttl, rtt);

    }
    else
    {
        fprintf(stderr, "Invalid ICMP packet! ");
        fprintf(stderr, "icmp_type:%d,! icmp->icmp_id:%d\n",  icmp->icmp_type , icmp->icmp_id );
        return -1;
    }
    return 0;
}



void* ping_recv(void *addr)
{
    struct sockaddr_in dest_sockaddr;
    struct timeval tv = {.tv_sec = 1, .tv_usec = 200};
    int ret = 0;
    fd_set read_fd;
    char recv_buf[512];
    socklen_t len = sizeof(struct sockaddr_in);
    while(alive)
    {
        FD_ZERO(&read_fd);
        FD_SET(pf_saw_icmp_socket, &read_fd);
        ret = select(pf_saw_icmp_socket+1, &read_fd, NULL, NULL, &tv);
        switch(ret)
        {
            case -1:
                fprintf(stderr,"fail to select!\n");
                break;
            case 0:
                break;
            default:
                {
                    //int size = recv(pf_saw_icmp_socket, recv_buf, sizeof(recv_buf), 0);
                    int size = recvfrom(pf_saw_icmp_socket, recv_buf, sizeof(recv_buf), 0, (struct sockaddr*)&dest_sockaddr, &len);
                    print_recv_ip(dest_sockaddr.sin_addr.s_addr, size);
                    if(size < 0)
                    {
                        fprintf(stderr,"recv data fail!\n");
                        continue;
                    }

                    ret = icmp_unpack(recv_buf, size); //对接收的包进行解封
                    if(ret == -1)  //不是属于自己的icmp包，丢弃不处理
                    {
                        continue;
                    }
                    recv_count++; //接收包计数
                }
                break;
        }
    }
    return NULL;
}

void icmp_sigint(int signo)
{
    printf(" recv signo:SIGINT:%d\n", signo);
    alive = 0;
}

void ping_stats_show()
{
    struct timeval end_time;
    gettimeofday(&end_time, NULL);
    long time = cal_time_offset(start_time, end_time);
    /*注意除数不能为零，这里send_count有可能为零，所以运行时提示错误*/
    printf("%d packets transmitted, %d recieved, %d%c packet loss, time %ldms\n",
            send_count, recv_count, (send_count-recv_count)*100/send_count, '%', time);
}

int getIcmpProtoNum()
{
    struct protoent* protocol  = getprotobyname("icmp"); //获取协议类型ICMP
    if(protocol == NULL)
    {
        printf("getprotobyname fail!\n");
        _exit(-1);
    }
    printf("Protocol number:%d\n", protocol->p_proto); //1
    return protocol->p_proto;
}



int set_dest_sock_addr(struct sockaddr_in *dest_socket_addr, const char*ip_or_domain)
{
    bzero(dest_socket_addr, sizeof(struct sockaddr_in));
    dest_socket_addr->sin_family = AF_INET;

    in_addr_t dest_addr = inet_addr(ip_or_domain);
    if(dest_addr == INADDR_NONE)   //判断用户输入的是否为IP地址还是域名
    {
        //输入的是域名地址
        struct hostent* dest_host_entry = gethostbyname(ip_or_domain);
        if(dest_host_entry == NULL)
        {
            printf("Fail to gethostbyname!\n");
            return -1;
        }
        printf("host entry name:%s\n", dest_host_entry->h_name);
        memcpy((char*)&dest_socket_addr->sin_addr, dest_host_entry->h_addr, dest_host_entry->h_length);
    }
    else
    {
        memcpy((char*)&dest_socket_addr->sin_addr, &dest_addr, sizeof(dest_addr));//输入的是IP地址
    }
    print_dest_ip(dest_socket_addr->sin_addr.s_addr, ip_or_domain);
    return 0;
}

void* ping_send(void *arg)
{
    struct sockaddr* dest_socket_addr = (struct sockaddr*)arg;
    char send_buf[ICMP_PACKET_PACKET_LEN];
    gettimeofday(&start_time, NULL); //记录第一个ping包发出的时间
    while(alive)
    {
        icmp_pack((struct icmp*)send_buf, send_count, ICMP_PACKET_PACKET_LEN); //封装icmp包
        int size = sendto(pf_saw_icmp_socket, send_buf, ICMP_PACKET_PACKET_LEN, 0, dest_socket_addr, sizeof(struct sockaddr_in));
        if(size < 0) {
            fprintf(stderr, "send icmp packet fail!\n");
            continue;
        }
        else{
            send_count++; //记录发出ping包的数量
        }

        sleep(1);
    }
    return NULL;
}


int main(int argc, char* argv[])
{
    if(argc < 2)
    {
        printf("usage %s <IP ADDRESS or domain>!\n", argv[0]);
        return -1;
    }
    printf("main pid:%d\n", getpid());
    printf("sizeof icmp:%lu\n", sizeof(struct icmp));
    printf("sizeof ip:%lu\n", sizeof(struct ip));
    printf("sizeof buff:%lu\n", ICMP_PACKET_PACKET_LEN);
    const char*ip_or_domain = argv[1];
    struct sockaddr_in dest_socket_addr;
    if (set_dest_sock_addr(&dest_socket_addr, ip_or_domain))
        return -1;

    pf_saw_icmp_socket = socket(PF_INET, SOCK_RAW, IPPROTO_ICMP);
    if(pf_saw_icmp_socket < 0)
    {
        printf("Fail to create socket!\n");
        return -1;
    }

    int so_rcvbuf = 128*1024;//128k
    setsockopt(pf_saw_icmp_socket, SOL_SOCKET, SO_RCVBUF, &so_rcvbuf, sizeof(so_rcvbuf)); //增大接收缓冲区至128K


    
    alive = 1;  //控制ping的发送和接收

    signal(SIGINT, icmp_sigint);

    pthread_t send_id,recv_id;
    pthread_create(&send_id, NULL, ping_send, &dest_socket_addr);
    pthread_create(&recv_id, NULL, ping_recv, &dest_socket_addr);
    pthread_join(send_id, NULL);//等待send ping线程结束后进程再结束
    pthread_join(recv_id, NULL);//等待recv ping线程结束后进程再结束

    ping_stats_show();
    close(pf_saw_icmp_socket);
    return 0;
}
