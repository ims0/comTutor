#include<gtest/gtest.h>
#include"createSocket.h"

class globalEnvironment : public testing::Environment{
public:
    virtual void SetUp()
    {
        std::cout << "============ global Environment SetUP    ========" << std::endl;
    }
    virtual void TearDown()
    {
        std::cout << "============ global Environment TearDown ========" << std::endl;
    }
};

int clientSocketFd;
int main(int argc, char **argv)
{
    testing::GTEST_FLAG(break_on_failure) = 0;
    testing::InitGoogleTest(&argc, argv);
    auto globalEnv = new globalEnvironment();
    testing::AddGlobalTestEnvironment(globalEnv) ;


    constexpr static const char *ip= "127.0.0.1";
    static const int ipPort = 3001;
    clientSocketFd = getSocketFd(ipPort, ip);
    int ret =  RUN_ALL_TESTS();
    close(clientSocketFd);
    return ret;
}
