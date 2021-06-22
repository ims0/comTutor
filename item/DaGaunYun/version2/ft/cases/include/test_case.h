
#include<gtest/gtest.h>
#include<iostream>
#include<enterFun.h>
using namespace std;



class TestCase:public testing::Test
{
public:
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

