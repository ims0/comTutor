/*************************************************************************
    > File Name: callAddFun.cpp
    > Author: ims
    > Created Time: 2019年07月02日 星期二 23时39分29秒
 ************************************************************************/
#include<iostream>
using namespace std;

extern int addFun(int a, int b);

void printFun()
{
    cout<<"addFun"<<addFun(1,2)<<endl;
}
