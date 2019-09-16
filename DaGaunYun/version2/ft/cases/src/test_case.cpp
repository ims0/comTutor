#include "test_case.h"

TEST_F(TestCase, hostNameErr)
{
    vector<string>hostName{"宝玉","贾母","error"};
    int wage = CalcWage("晴雯","4",hostName);
    ASSERT_EQ(wage, 1);
}
TEST_F(TestCase, ServantNameErr)
{
    vector<string>hostName{"宝玉","贾母","贾政"};
    int wage = CalcWage("晴雯err","4",hostName);
    ASSERT_EQ(wage, 1);
}

TEST_F(TestCase, Test_four)
{
    vector<string>hostName{"宝玉","贾母","贾政"};
    int wage = CalcWage("晴雯","4",hostName);
    ASSERT_EQ(wage, 23600);
}
TEST_F(TestCase, SheYue_test)
{
    vector<string>hostName{"宝玉","贾母","贾政"};
    int wage = CalcWage("麝月","4",hostName);
    ASSERT_EQ(wage, 21200);
}
TEST_F(TestCase, XiRen_test)
{
    vector<string>hostName{"宝玉","贾母","贾政"};
    int wage = CalcWage("袭人","4",hostName);
    ASSERT_EQ(wage, 58000);
}
