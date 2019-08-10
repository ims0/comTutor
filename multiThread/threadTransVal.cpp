/*************************************************************************
    > File Name: threadTransVal.cpp
    > Author: ims
    > Created Time: 2019年08月10日 星期六 20时30分07秒
 ************************************************************************/

#include<iostream>
#include<string>
#include<thread>
using namespace std;


void print( int i, const string &str)
{
    i =10;
    cout<<i<<endl;
    cout<<str.c_str()<<endl;

    return;
}
int main()
{
    int mvar = 1;
    int mbuf[] = { 1,2,3,4,5 };
    char *ss = "hello";
    thread mObj(print ,mvar, ss);

    mObj.join();
    //mObj.detach();
    cout<<"end!"<<endl;
    cout<<"end!"<<endl;
    
    cout<<"i:"<< mvar<<endl;
    return 0;
}
