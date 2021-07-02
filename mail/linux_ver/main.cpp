#include "smtp.h"
#include "signal.h"

int main()
{
    signal(SIGPIPE, SIG_IGN);
    Csmtp mail(
        25,
        "smtp.126.com",
        "o@126.com",// 来源邮箱
        "i",
        "o@126.com" //目标邮箱
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
    //dd if=/dev/zero of=file_dd bs=1024 count=2000
    mail.addfile("file_dd"); //添加附件

    mail.SendMail();
    return 0;
}
