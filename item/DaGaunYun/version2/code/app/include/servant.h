#ifndef SERVANT_H
#define SERVANT_H
#include <functional>
class Servant;
typedef std::function<int (const Servant&)>WageCalcFunc;
class Servant{
public:
    int wage(WageCalcFunc wageFunc,int quar,int mon){
        month = mon;
        quarter = quar;
        return wageFunc(*this);
    }
    virtual int getInstMoney()const = 0;
    virtual int getRewardMoney()const = 0;
    virtual ~Servant(){}

    int month;
    int quarter;
};

class QingWen:public Servant
{

public:
    int getInstMoney()const
    {
        return month%2 != 0 ? singleMonthInstMoney:doubleMonthInstMoney;
    }
    int getRewardMoney()const
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
    int getInstMoney()const
    {
        return constInstMoney;
    }
    int getRewardMoney()const
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
    int getInstMoney()const
    {
        return quarter%2 !=0 ? singleQuarterInstMoney :doubleQuarterInstMoney;
    }
    int getRewardMoney()const
    {
        return constRewardMoney;
    }
private:
    const int singleQuarterInstMoney = 1000;
    const int doubleQuarterInstMoney = 1500;
    const int constRewardMoney = 2000;
};

#endif
