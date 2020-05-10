#include<gtest/gtest.h>
#include<iostream>
#include"createSocket.h"

using namespace std;

class MsgTest:public testing::Test
{
    constexpr static const char *ip= "127.0.0.1";
    static const int ipPort = 3001;
public:
    static int socketFd;
    MsgTest()
    {
        cout<<"------ create -------"<<endl;
    }
    static void SetUpTestCase()
    {
        cout<<"------ static SetUpTestCase() -------"<<endl;
        socketFd = getSocketFd(ipPort, ip);
        ASSERT_GT(socketFd, 1);
    }
 
    static void TearDownTestCase()
    {
        close(socketFd);
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

int MsgTest::socketFd = 0;
TEST_F(MsgTest, hostNameErr)
{
    char buf[256];
    bzero(buf,sizeof(buf));
    const char *str="first test!";
    send(MsgTest::socketFd, str, strlen(str), 0);
    recv(MsgTest::socketFd, buf, 256,0);

    printf("send:%s, recv:%s\n",str, buf);
    ASSERT_EQ(strcmp(str,buf), 0);
}

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
