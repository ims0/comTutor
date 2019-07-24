#include "test_case.h"

int CalcWage(string servantName,string quarter);
TEST_F(TestCase, Test1)
{
    cout<<"wage:"<<CalcWage("袭人","1")<<endl;
    ASSERT_EQ(CalcWage("袭人","1"), 9000);
}
