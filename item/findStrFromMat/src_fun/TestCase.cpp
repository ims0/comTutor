
#include <gtest/gtest.h>
#include "../include/findstr.h"

TEST(FooTest, RequireCase)
{
    EXPECT_EQ(findStr("teca heht eerx phzM", "teeth peer cat hei"),1);
}
TEST(FooTest, SingleCharCase)
{
    EXPECT_EQ(findStr("qwer asdf zxcv", "s a d"),0);
}
TEST(FooTest, multiStrCase)
{
    EXPECT_EQ(findStr("qwer asdf zxcv", "qwsxcv    qwedsazxcv    rfvcxzaswq"),0);
}
TEST(FooTest, UseTwiceiSameChar)
{
	EXPECT_EQ(findStr("qwer asdf zxcv", "aa cvc qazxz"),1);
}

TEST(FooTest, NotSerieschar)
{
	EXPECT_EQ(findStr("qwer asdf zxcv", "qsc asf zxdf qwdf"),1);
}

