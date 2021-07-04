#include "smtp.h"
#include "signal.h"

int main(int argc, char*argv[])
{
    signal(SIGPIPE, SIG_IGN);
    Csmtp mail(
        25,
        "smtp.126.com",
        "@126.com",// 来源邮箱
        "",
        "@126.com" //目标邮箱
        );

    if (mail.connectServer())
    {
        cout << "connect Server succ!" << endl;
    }
    else{
        cout << "connect Server failed!" << endl;
        return -1;//
    }

    mail.setTitle("test mail");
    mail.setContent("this is content.");
    //附件路径错误，不影响邮件正文的发送。
    //dd if=/dev/zero of=file_dd bs=1024 count=2000
    while(argc >= 2)
    {
        cout<<argv[--argc]<<endl;
        mail.addfile(argv[argc]);
    }
    mail.SendMail();
    return 0;
}
