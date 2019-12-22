/*************************************************************************
    > File Name: main.c
    > Author: ims
    > Created Time: 2019年05月17日 星期五 23时12分20秒
 ************************************************************************/




#include<stdio.h>
#include"test.h"
#include"fun.h"
int main()
{
    double b=MACRO25;
    double a=0.0;
    a=get_sqrt(b);
 
    printf("a is %lf, b is %lf\n",a,b);
    return 0;
}

