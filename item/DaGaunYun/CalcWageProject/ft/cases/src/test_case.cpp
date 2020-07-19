#include "test_case.h"

int CalcWage(string servantName,string quarter);
TEST_F(TestCase, Test1)
{
    cout<<"wage:"<<CalcWage("袭人","1")<<endl;
    ASSERT_EQ(CalcWage("袭人","1"), 9000);
}
TEST_F(TestCase, Test_four)
{
    vector<string>hostName{"宝玉","贾母","贾政"}; 
    vector<string>score{"7","8","9"}; 
    int wage = CalcWage("晴雯","4",hostName,score);
    ASSERT_EQ(wage, 24600);
}
