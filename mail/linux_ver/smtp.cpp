#include "smtp.h"
#include "string.h"
#include <arpa/inet.h>

extern string base64Encode(char const* origSigned, unsigned origLength);

static char s_recv_buff[100];  //recv函数返回的结果
void check_errno(int ret)
{
    if (ret < 0)
    {
        cerr<<"send failed! exit." <<endl;
        exit(-1);
    }
    else
    {
        cerr<<"send len:" << ret <<endl;
    }
}
#define IP_SIZE     16
const int smtp_max_attach = 1000*1000;
int Csmtp::SendAttachment(int &_sockfd) /*发送附件*/
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
        send(_sockfd, sendstring.c_str(), sendstring.length(), 0);

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

        string encodeBuff = base64Encode(buffer, length);
        delete[]buffer;
        if( encodeBuff.length() > smtp_max_attach )
        {
            int send_len = 0;
            char*offset = const_cast<char*>(encodeBuff.c_str());
            const char *end = offset + encodeBuff.length();

            for( ; offset < end ; offset += smtp_max_attach )
            {
                int len = end- offset > smtp_max_attach ? smtp_max_attach :end- offset > smtp_max_attach;
                send_len = send(_sockfd, offset, len, 0);
                check_errno(send_len);
                send_len = send(_sockfd, "\r\n", strlen("\r\n"), 0);
                check_errno(send_len);
                offset += smtp_max_attach;
            }
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
    //printHost(hptr);
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
    char serverIp[16];
    getIpByDomain(_domain.c_str(), serverIp);
    printf("mail server ip:%s\n", serverIp);

    //init serverAddr
    struct sockaddr_in serverAddr;
    bzero(&serverAddr,sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(_port);
    if(inet_pton(AF_INET, serverIp, &serverAddr.sin_addr) <= 0 )
    {
        printf("inet_pton error!\n");
        return -1;
    };
    // create _sockfd and connnect
    _sockfd = socket(AF_INET,SOCK_STREAM,0);
    if(_sockfd < 0)
    {
        printf("Create socket error!\n");
        return -1;
    }
    puts("start connect!");
    if(connect(_sockfd,(struct sockaddr *)&serverAddr,sizeof(serverAddr)) < 0)
    {
        printf("Connect failed... \n");
        return -1;
    }
    else{
        printf("Connect to %s .... \n", serverIp);
    }
    char buff[100];  //recv函数返回的结果
    buff[recv(_sockfd, buff, 500, 0)] = '\0';
    cout<<"connect response.[rcv]:"<<buff<<endl;
    return true;
}


void Csmtp::_recv(const char*str)
{
    s_recv_buff[recv(_sockfd, s_recv_buff, 500, 0)] = '\0';
    cout<<str<<":"<<s_recv_buff<<endl;
    if( strstr(s_recv_buff, "Error") )
    {
        cerr<<"exit!"<<endl;
        exit(EXIT_FAILURE);
    }
}
int Csmtp::_sendCmd(const char *cmd)
{
    return send(_sockfd, cmd, strlen(cmd), 0);
}
int Csmtp::SendMail()
{
    _sendCmd("ehlo 126.com\r\n");
    _recv("helo");

    _sendCmd("auth login\r\n");
    _recv("auth login");

    //上传邮箱名
    string message = base64Encode(_sender.c_str(), _sender.length()) ;
    _sendCmd(message.c_str());
    _recv("auth login");
    //上传邮箱密码
    message = base64Encode(_pswd.c_str(), _pswd.length()) ;
    _sendCmd(message.c_str());
     _recv("password.[rvc]:");

    message = "mail from:<" + _sender + ">\r\n";
    _sendCmd(message.c_str());
    _recv("mail from");
    message = "rcpt to:<" + _recver + ">\r\n";
    _sendCmd(message.c_str());
    _recv("rcpt from");

    message = "data\r\n"; /*data要单独发送一次*/
    _sendCmd(message.c_str());
    _recv("data rsp");
    
    //要使用Csmtp 发送附件, 需要对Csmtp 头信息进行说明, 改变Content-type 及为每一段正文添加BOUNDARY 名,
    cout << "-----------DATA--------------" << endl;
    message = "from:" + _sender + "\r\nto:" + _recver + "\r\nsubject:" + _title + "\r\n";
    message += "MIME-Version: 1.0\r\n";
    message += "Content-Type: multipart/mixed;boundary=@boundary@\r\n\r\n";
    _sendCmd(message.c_str());

    //  正文
    message = "--@boundary@\r\nContent-Type: text/plain;charset=\"gb2312\"\r\n\r\n" + _content + "\r\n\r\n";
    _sendCmd(message.c_str());

    //  发送附件
    SendAttachment(_sockfd);

    /*发送结尾信息*/
    message = "--@boundary@--\r\n.\r\n";
    _sendCmd(message.c_str());
    _recv("end_qwerty.[rcv]:");

    message = "QUIT\r\n";
    _sendCmd(message.c_str());
    _recv("end_qwerty.[rcv]:");
    return 0;
}
