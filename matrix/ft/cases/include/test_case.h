#ifndef TEST_CASE_H
#define TEST_CASE_H
#include<gtest/gtest.h>
#include<iostream>
using namespace std;


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

#endif
