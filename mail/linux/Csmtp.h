#include <iostream>  
#include <string>  
#include <vector>
#include <fstream>  

#include<errno.h>  
#include<sys/types.h>  
#include<sys/socket.h>  
#include<netinet/in.h>
#include<netdb.h>
#pragma  comment(lib, "ws2_32.lib") /*链接ws2_32.lib动态链接库*/ 
// POP3服务器（端口：110） Csmtp服务器（端口：25） 
using namespace std;


class Csmtp
{

    int port;
    string domain;
    string user;
    string pass;
    string target;
    string title;  //邮件标题
    string content;  //邮件内容


    hostent* pHostent;
    int sockfd;  //客户端的套接字
    vector <string> filename;  //存储附件名的向量

public:

    Csmtp(
        int _port, //端口25
        string _domain,     //域名
        string _user,       //发送者的邮箱
        string _pass,       //密码
        string _target)     //目标邮箱
        :port(_port), domain(_domain), user(_user), pass(_pass), target(_target){};//内容 
    bool connectServer();
    void setTitle(string tem){ title = tem; }
    void setContent(string tem){ content = tem; }

    int SendAttachment(int  &sockClient);
    int SendMail();
    void addfile(string str){ filename.push_back(str); }

};
