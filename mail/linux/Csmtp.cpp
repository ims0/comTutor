#include "Csmtp.h"
#include "string.h"
#include<arpa/inet.h>
extern char* base64Encode(char const* origSigned, unsigned origLength);

#define IP_SIZE     16

int Csmtp::SendAttachment(int &sockfd) /*发送附件*/
{
    for (std::vector<string>::iterator iter = filename.begin(); iter != filename.end(); ++iter)
    {
        cout << "Attachment is sending··· " << endl;

        string path = *iter;
        ifstream ifs(path, ios::in | ios::binary);
        if (false == ifs.is_open())
        {
            cout << "无法打开文件！" << endl;
            return 1;
        }

        string sendstring;
        sendstring = "--@boundary@\r\nContent-Type: application/octet-stream; name=\""+path+"\"\r\n";
        sendstring += "Content-Disposition: attachment; filename=\""+path+"\"\r\n";
        sendstring += "Content-Transfer-Encoding: base64\r\n\r\n";
        send(sockfd, sendstring.c_str(), sendstring.length(), 0);

        //infile.read((char*)buffer,sizeof(数据类型));

        // get length of file:
        ifs.seekg(0, ifs.end);
        int length = ifs.tellg();
        ifs.seekg(0, ifs.beg);
        cout << "length:" << length << endl;
        // allocate memory:
        char * buffer = new char[length];
        // read data as a block:
        ifs.read(buffer, length);
        ifs.close();
        char *pbase;
        pbase = base64Encode(buffer, length);
        delete[]buffer;
        string str(pbase);
        delete[]pbase;
        str += "\r\n";
        int err = send(sockfd, str.c_str(), strlen(str.c_str()), 0);

        if (err != (int)strlen(str.c_str()))
        {
            cout << "附件发送出错!" << endl;
            return 1;
        }
    }
    return 0;
}

#if 0
struct hostent {
    char  *h_name;            /* official name of host */
    char **h_aliases;         /* alias list */
    int    h_addrtype;        /* host address type */
    int    h_length;          /* length of address */
    char **h_addr_list;       /* list of addresses */
}
#endif

void printHost(struct hostent * host)
{
    printf("host.h_name:%s\n", host->h_name);
    printf("host.h_length:%d \n", host->h_length);
    printf("host.h_addr_list:%s \n", host->h_addr_list[0]);
    printf("host.h_addr_list:%s \n", host->h_addr_list[1]);
}
int getIpByDomain(const char *domain, char *ip)
{
    char **pptr;
    struct hostent *hptr;

    puts("get host");
    hptr = gethostbyname(domain);
    if(NULL == hptr)
    {
        printf("gethostbyname error for host:%s \n", domain);
        return -1;
    }
    printHost(hptr);
    for(pptr = hptr->h_addr_list ; *pptr != NULL; pptr++)
    {
        if(NULL != inet_ntop(hptr->h_addrtype, *pptr, ip, IP_SIZE) )
        {
            return 0; // 只获取第一个 ip
        }
    }

    return -1;
}


bool Csmtp::connectServer()
{

    ///////////////////  connect to server /////////////////////////////////////
    char serverIp[16];
    getIpByDomain(domain.c_str(), serverIp);

    //init serverAddr
    struct sockaddr_in serverAddr;
    bzero(&serverAddr,sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    if(inet_pton(AF_INET, serverIp, &serverAddr.sin_addr) <= 0 )
    {
        printf("inet_pton error!\n");
        return -1;
    };
    // create sockfd and connnect
    sockfd = socket(AF_INET,SOCK_STREAM,0);
    if(sockfd < 0)
    {
        printf("Create socket error!\n");
        return -1;
    }
    if(connect(sockfd,(struct sockaddr *)&serverAddr,sizeof(serverAddr)) < 0)
    {
        printf("Connect failed... \n");
        return -1;
    }
    else{
        printf("Connect to %s .... \n", serverIp);
    }
    char buff[100];  //recv函数返回的结果
    buff[recv(sockfd, buff, 500, 0)] = '\0';
    cout<<"connect response.[rcv]:"<<buff<<endl;
    return true;
}


int Csmtp::SendMail()
{
    char *ecode;

    char buff[500];  //recv函数返回的结果
    string message; //

    message = "ehlo 126.com\r\n";
    send(sockfd, message.c_str(), message.length(), 0);

    buff[recv(sockfd, buff, 500, 0)] = '\0';
    cout<<"helo:"<<buff<<endl;

    message = "auth login \r\n";
    send(sockfd, message.c_str(), message.length(), 0);
    buff[recv(sockfd, buff, 500, 0)] = '\0';
    cout<<"auth login.[rvc]:"<<buff<<endl;
    //上传邮箱名
    message = user;
    ecode = base64Encode(message.c_str(), strlen(message.c_str()));
    message = ecode;
    message += "\r\n";
    delete[]ecode;
    send(sockfd, message.c_str(), message.length(), 0);
    buff[recv(sockfd, buff, 500, 0)] = '\0';
    cout<<"usrname.[rvc]:"<<buff<<endl;
    //上传邮箱密码
    message = pass;
    ecode = base64Encode(message.c_str(), strlen(message.c_str()));
    message = ecode;
    delete[]ecode;
    message += "\r\n";
    send(sockfd, message.c_str(), message.length(), 0);
    buff[recv(sockfd, buff, 500, 0)] = '\0';
    cout<<"password.[rvc]:"<<buff<<endl;
    cout<<"------------ login finish --------------"<<endl<<endl;
    message = "mail from:<" + user + ">\r\nrcpt to:<" + target + ">\r\n";
    send(sockfd, message.c_str(), message.length(), 0);
    buff[recv(sockfd, buff, 500, 0)] = '\0';
    cout<<"mail from: "<<buff<<endl;
    buff[recv(sockfd, buff, 500, 0)] = '\0';
    cout<<"rcpt to: "<<buff<<endl;

    message = "data\r\n";//data要单独发送一次
    send(sockfd, message.c_str(), message.length(), 0);
    buff[recv(sockfd, buff, 500, 0)] = '\0';
    cout<<"data: "<<buff<<endl;
    ///-----------------------------------------DATA-------------------------------------
    //要使用Csmtp 发送附件, 需要对Csmtp 头信息进行说明, 改变Content-type 及为每一段正文添加BOUNDARY 名,
    cout << "-------------------DATA------------------------" << endl;
    //  头
    message = "from:" + user + "\r\nto:" + target + "\r\nsubject:" + title + "\r\n";
    message += "MIME-Version: 1.0\r\n";
    message += "Content-Type: multipart/mixed;boundary=@boundary@\r\n\r\n";
    send(sockfd, message.c_str(), message.length(), 0);

    //  正文
    message = "--@boundary@\r\nContent-Type: text/plain;charset=\"gb2312\"\r\n\r\n" + content + "\r\n\r\n";
    send(sockfd, message.c_str(), message.length(), 0);

    //------------------------------------------------------------------------------------------------
    //  发送附件

    SendAttachment(sockfd);

    /*发送结尾信息*/
    message = "--@boundary@--\r\n.\r\n";
    send(sockfd, message.c_str(), message.length(), 0);
    buff[recv(sockfd, buff, 500, 0)] = '\0';
    cout<<"end_qwerty.[rcv]:"<<buff<<endl;

    message = "QUIT\r\n";
    send(sockfd, message.c_str(), message.length(), 0);
    buff[recv(sockfd, buff, 500, 0)] = '\0';
    cout << "Send mail is finish.[rcv]:" << buff << endl;
    return 0;
}
