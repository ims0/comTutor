/*************************************************************************
    > File Name: main.cpp
    > Author: ims
    > Created Time: 2019年07月23日 星期二 23时51分47秒
 ************************************************************************/

#include<gtest/gtest.h>

int main(int argc, char **argv)
{
    testing::GTEST_FLAG(break_on_failure) = 0;
    testing::InitGoogleTest(&argc, argv) ;
    return RUN_ALl_TESTS();
}
