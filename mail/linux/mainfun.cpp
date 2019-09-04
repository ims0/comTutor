#include "Csmtp.h"

int main()
{

    Csmtp mail(
        25,
        "smtp.126.com",
        "xxx@126.com",// 来源邮箱
        "xxx",
        "xxx.com" //目标邮箱
        );

    if (!mail.CReateSocket())
    {
        cout << "ReateSocket failed!" << endl;
        return -1;//
    }
    //标题默认是主机名，内容默认是ip
    mail.setTitle("test mail");
    mail.setContent("this is content.");
    //附件路径错误，不影响邮件正文的发送。
    mail.addfile("test.jpg"); //添加附件
    //mail.addfile("test2.png"); //添加附件

    mail.SendMail(); //类主函数
    system("pause");
    return 0;
}
