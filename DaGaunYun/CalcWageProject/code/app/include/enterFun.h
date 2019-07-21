/*************************************************************************
    > File Name: CalcWageProject/code/app/include/enterFun.h
    > Author: ims
    > Created Time: 2019年07月21日 星期日 13时30分28秒
 ************************************************************************/

#ifndef ENTERFUN_H
#define ENTERFUN_H

#include"servant.h"
#include"host.h"
#include <string>
#include <vector>
 
using std::string;
using std::vector;
int CalcWage(string servantName,string quarter);

int CalcWage(string servantName, string quarterStr, string hostName);
int CalcWage(string servantName, string quarterStr, 
        const vector<string>&hostName);

int CalcWage(string servantName, string quarterStr, 
        const vector<string>&hostName ,const vector<string>&score);
#endif
 
