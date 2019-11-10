/*************************************************************************
    > File Name: main.cpp
    > Author: ims
    > Created Time: 2019年06月26日 星期三 21时56分22秒
 ************************************************************************/

#include"enterFun.h"
#include<iostream>
using namespace std;

int main(int argc, char*argv[])
{

    cout<<"argc:"<<argc<<endl;

    if( argc == 3 )
    {
        cout<<"grade 1 wage:"<<CalcWage(argv[1], argv[2])<<endl;
    }
    if( argc == 4 )
    {
        cout<<"grade 2 wage:"<<CalcWage(argv[1], argv[2], argv[3])<<endl;
    }
    if( argc == 6 )
    {
        vector<string>hostName;
        hostName.push_back(argv[3]);
        hostName.push_back(argv[4]);
        hostName.push_back(argv[5]);
        cout<<"grade 3 wage:"<<CalcWage(argv[1], argv[2], hostName)<<endl;
    }
    if( argc == 9 )
    {
        vector<string>hostName;
        hostName.push_back(argv[3]);
        hostName.push_back(argv[4]);
        hostName.push_back(argv[5]);
        vector<string>score;
        score.push_back(argv[6]);
        score.push_back(argv[7]);
        score.push_back(argv[8]);
        cout<<"grade 4 wage:"<<CalcWage(argv[1], argv[2], hostName,score)<<endl;
    }
    return 0;
}
