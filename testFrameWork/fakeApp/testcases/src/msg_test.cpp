#include<gtest/gtest.h>
#include<iostream>

using namespace std;

class MsgTest:public testing::Test
{
public:
    MsgTest()
    {
        cout<<"------ create -------"<<endl;
    }
    static void SetUpTestCase()
    {
        cout<<"------ static SetUpTestCase() -------"<<endl;
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
    ASSERT_EQ(1, 1);
}
TEST_F(MsgTest, hostNameErr1)
{
    ASSERT_EQ(1, 1);
}
