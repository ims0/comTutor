
#include<gtest/gtest.h>
#include<iostream>
#include<enterFun.h>
using namespace std;


class FooEnvironment : public testing::Environment{
public:
    virtual void SetUp()
    {
        std::cout << "Foo FooEnvironment SetUP" << std::endl;
    }
    virtual void TearDown()
    {
        std::cout << "Foo FooEnvironment TearDown" << std::endl;
    }
};


class TestCase:public testing::Test
{
public:
    static void SetUpTestCase()
    {
        cout<<"SetUpTestCase()"<<endl;
    }
 
    static void TearDownTestCase()
    {
        cout<<"TearDownTestCase()"<<endl;
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

