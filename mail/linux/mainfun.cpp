#include "Csmtp.h"

int main()
{

    Csmtp mail(
        25,
        "smtp.126.com",
        "xxx@126.com",// 来源邮箱
        "xxx",
        "xxx@126.com" //目标邮箱
        );

    if (!mail.connectServer())
    {
        cout << "connect Server failed!" << endl;
        return -1;//
    }
    else{

        cout << "connect Server succ!" << endl;
    }

    //标题默认是主机名，内容默认是ip
    mail.setTitle("test mail");
    mail.setContent("this is content.");
    //附件路径错误，不影响邮件正文的发送。
    mail.addfile("mainfun.cpp"); //添加附件

    mail.SendMail(); //类主函数
    return 0;
}
