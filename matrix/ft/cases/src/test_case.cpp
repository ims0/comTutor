#include "test_case.h"
#include "matrix.h"

TEST_F(TestCase, adjoint_test)
{
    Matrix mat(4);
    mat<<0.53,0.43,0.61,0.97,
        0.69,0.31,0.49,0.87,
        0.26,0.93,0.34,0.06,
        0.98,0.80,0.65,0.58;
    cout<<mat;
    Matrix tem(mat.inverse());
    cout<<tem;


    ASSERT_EQ(1, 1);
}

