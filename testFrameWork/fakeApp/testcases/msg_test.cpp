#include<gtest/gtest.h>
#include<iostream>
#include"createSocket.h"
#include "pubsub.h"

using namespace std;

extern int clientSocketFd;
class MsgTest:public testing::Test
{
    constexpr static const char *ip= "127.0.0.1";
    static const int ipPort = 3001;
public:
    MsgTest()
    {
        cout<<"------ create -------"<<endl;
    }
    static void SetUpTestCase()
    {
        cout<<"------ static SetUpTestCase() -------"<<endl;
        ASSERT_GT(clientSocketFd, 1);
    }
 
    static void TearDownTestCase()
    {
        cout<<"------- static TearDownTestCase() --------"<<endl;
    }
    void SetUp()
    {
        cout<<"SetUp() is running"<<endl;
         
    }
    void TearDown()
    {
        cout<<"TearDown()"<<endl;
    } 
};

TEST_F(MsgTest, hostNameErr)
{
#define rcvBufLen (256)
    cout<<"clientSocketFd:"<<clientSocketFd<<endl;
    char buf[rcvBufLen];
    bzero(buf,sizeof(buf));
    const char *str="first test!";
    PSMsg psBrief={0};
    psBrief.msgId = 1;
    op_pub(&psBrief, str, strlen(str));
    //send(MsgTest::socketFd, str, strlen(str), 0);
    getMsg(buf, rcvBufLen);

    printf("send:%s, recv:%s\n",str, buf);
    //PSMsg *revMsg = static_cast<PSMsg*>(buf);
    PSMsg *revMsg = (PSMsg*)(buf);
    ASSERT_EQ(psBrief.msgId, revMsg->msgId);
}

#if 0
TEST_F(MsgTest, hostNameErr1)
{
    char buf[256];
    bzero(buf,sizeof(buf));
    const char *str="scend test!";
    send(MsgTest::socketFd, str, strlen(str), 0);
    recv(MsgTest::socketFd, buf, 256,0);

    printf("send:%s, recv:%s\n",str, buf);
    ASSERT_EQ(strcmp(str,buf), 0);
}
#endif
