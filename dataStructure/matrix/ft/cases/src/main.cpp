
#include<test_case.h>

int main(int argc, char **argv)
{
    testing::GTEST_FLAG(break_on_failure) = 0;
    testing::InitGoogleTest(&argc, argv) ;
    testing::AddGlobalTestEnvironment(new globalEnvironment()) ;
    return RUN_ALL_TESTS();
}
