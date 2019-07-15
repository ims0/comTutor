/*************************************************************************
    > File Name: servant.h
    > Author: ims
    > Created Time: 2019年07月15日 星期一 23时05分13秒
 ************************************************************************/

#ifndef SERVANT_H
#define SERVANT_H

class Servant{
Public:
    int CalcServantWage(int quarter)
    {
        int quarterWage = 0;
        int monthStart = (quarter-1)*3 +1;
        for(int monthLoop = monthStart; monthLoop < monthStart+3 ; ++monthLoop)
        {
            quarterWage += getInstMoney()+getRewardMoney();
        }
        return quarterWage;
    }
    virtual int getInstMoney(int quarter, int month)const = 0;
    virtual int getRewardMoney(int quarter, int month)const = 0;
    virtual ~Servant(){ }
};

class QingWen:public Servant
{

public:
    int getInstMoney(int quarter, int month)const
    {
        return month%2 != 0 ? singleMonthInstMoney:doubleMonthInstMoney;
    }
    int getRewardMoney(int quarter, int month)const
    {
        return constRewardMoney;
    }
private:
    const int singleMonthInstMoney = 900;
    const int doubleMonthInstMoney = 800;
    const int constRewardMoney = 500;
};
class SheYue: public Servant
{

public:
    int getInstMoney(int quarter, int month)const
    {
        return constInstMoney;
    }
    int getRewardMoney(int quarter, int month)const
    {
        return quarter<3? firstHalfYearRewardMoney:secHalfYearRewardMoney;
    }
private:
    const int constInstMoney = 600;
    const int firstHalfYearRewardMoney = 400;
    const int  secHalfYearRewardMoney = 700;
};
class XiRen:public Servant
{

public:
    int getInstMoney(int quarter, int month)const
    {
        return quarter%2 !=0 ? singleQuarterInstMoney :doubleQuarterInstMoney;
    }
    int getRewardMoney(int quarter, int month)const
    {
        return constRewardMoney;
    }
private:
    const int singleQuarterInstMoney = 1000;
    const int doubleQuarterInstMoney = 1500;
    const int constRewardMoney = 2000;
};

#endif
