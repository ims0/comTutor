#include<gtest/gtest.h>

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

int main(int argc, char **argv)
{
    testing::GTEST_FLAG(break_on_failure) = 0;
    testing::InitGoogleTest(&argc, argv);
    auto globalEnv = new globalEnvironment();
    testing::AddGlobalTestEnvironment(globalEnv) ;
    int ret =  RUN_ALL_TESTS();
    return ret;
}
