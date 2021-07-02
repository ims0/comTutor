#include <iostream>  
#include <string>  
#include <vector>
#include <fstream>  
#include <errno.h>  
#include <sys/types.h>  
#include <sys/socket.h>  
#include <netinet/in.h>
#include <netdb.h>
// POP3服务器（端口：110） Csmtp服务器（端口：25） 
using namespace std;

class Csmtp
{
    int _port;
    int _sockfd;
    string _domain;
    string _sender;
    string _pswd;
    string _recver;
    string _title;  //邮件标题
    string _content;  //邮件内容

    hostent* pHostent;
    vector <string> filename;  //存储附件名的向量

    void _recv(const char*);
    int _sendCmd(const char *cmd);
public:
    Csmtp(int port, string domain, string sender, string pswd, string recver)
        :_port(port), _domain(domain), _sender(sender), _pswd(pswd), _recver(recver){};//内容 
    bool connectServer();
    void setTitle(string tem){ _title = tem; }
    void setContent(string tem){ _content = tem; }

    int SendAttachment(int  &sockClient);
    int SendMail();
    void addfile(string str){ filename.push_back(str); }
};
