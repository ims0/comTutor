#include "smtp.h"
#include "string.h"
#include <arpa/inet.h>
#include <fcntl.h>

extern string base64Encode(char const* origSigned, unsigned origLength);
extern bool nb_connect(const char *host, int port);
#define FILE_BUF_LEN 700*1000
#define RECV_BUF_LEN 500
static char s_recv_buff[RECV_BUF_LEN];  //recv函数返回的结果
static char s_file_buff[FILE_BUF_LEN];

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
int Csmtp::SendAttachment(int &_sockfd) /*发送附件*/
{
    for (std::vector<string>::iterator iter = filename.begin(); iter != filename.end(); ++iter)
    {
        cout << "Attachment is sending··· " << endl;

        string path = *iter;
        ifstream ifs(path, ios::in | ios::binary);
        if (!ifs)
        {
            cout << "无法打开文件！" << endl;
            return 1;
        }

        string sendstring;
        sendstring = "--@boundary@\r\nContent-Type: application/octet-stream; name=\""+path+"\"\r\n";
        sendstring += "Content-Disposition: attachment; filename=\""+path+"\"\r\n";
        sendstring += "Content-Transfer-Encoding: base64\r\n\r\n";
        send(_sockfd, sendstring.c_str(), sendstring.length(), 0);

        int send_len = 0;
        while (ifs) {
            ifs.read(s_file_buff, FILE_BUF_LEN );
            cout << "file length:" << ifs.gcount() << endl;
            string encodeBuff = base64Encode(s_file_buff, ifs.gcount());
            send_len = send(_sockfd, encodeBuff.c_str(), encodeBuff.length(), 0);
            check_errno(send_len);
        }
        ifs.close();
    }
    return 0;
}

bool Csmtp::connectServer()
{
    if((_sockfd = nb_connect(_domain.c_str(), _port))) 
    {
        _recv("connect response.[rcv]:");
        return true;
    }
    else 
        return false;
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
    _recv("helo.[rvc]:");

    _sendCmd("auth login\r\n");
    _recv("auth login.[rvc]:");

    //上传邮箱名
    string message = base64Encode(_sender.c_str(), _sender.length()) ;
    _sendCmd(message.c_str());
    _recv("user.[rvc]:");
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
