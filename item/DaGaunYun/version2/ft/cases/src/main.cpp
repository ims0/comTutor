/*************************************************************************
    > File Name: main.cpp
    > Author: ims
    > Created Time: 2019年07月23日 星期二 23时51分47秒
 ************************************************************************/

#include<gtest/gtest.h>
#include<test_case.h>
class globalEnvironment : public testing::Environment{
public:
    virtual void SetUp()
    {
        std::cout << "======= global Environment SetUP ========" << std::endl;
    }
    virtual void TearDown()
    {
        std::cout << "======= global Environment TearDown =========" << std::endl;
    }
};
int main(int argc, char **argv)
{
    testing::GTEST_FLAG(break_on_failure) = 0;
    testing::InitGoogleTest(&argc, argv) ;
    testing::AddGlobalTestEnvironment(new globalEnvironment()) ;
    return RUN_ALL_TESTS();
}
