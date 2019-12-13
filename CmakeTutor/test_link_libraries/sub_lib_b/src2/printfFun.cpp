#include<iostream>
using namespace std;

extern int add(int a, int b);

void printFun2()
{
    cout<<__func__<<" call add func in libadd:"<<add(1,2)<<endl;
}
