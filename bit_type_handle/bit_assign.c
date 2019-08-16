/*************************************************************************
    > File Name: c.c
    > Author: ims
    > Created Time: 2019年05月21日 星期二 00时03分57秒
 ************************************************************************/
#include<stdio.h>

struct bit{
    unsigned a:3;
    unsigned b:5;
    unsigned c:8;
}A;

void fun(unsigned var)
{
    A.a= var&0b111 ;
    A.b= var&0B11111 ;
    A.c= var&0xff ;
    printf("A.a:%u\n", A.a);
    printf("A.b:%u\n", A.b);
    printf("A.c:%u\n", A.c);
}

int main()
{
    unsigned var =0XFF;
    fun(var);
    printf("var:%u \n", var);
    return 0;
}
