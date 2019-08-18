#include<wageCalcFunc.h>
#include<servant.h>

#if 0
int defaultWageCalcFunc(const Servant&serObj)
{
    return  serObj.getInstMoney()+serObj.getRewardMoney();
}
#endif

int baoYuWageFunc(const Servant&serObj)
{
    return  8 * (serObj.getInstMoney()+serObj.getRewardMoney());
}

int jiaMuWageFunc(const Servant&serObj)
{
    int instMoney = serObj.getInstMoney();
    int rewardMoney = serObj.getRewardMoney();
    return  10 * (instMoney > rewardMoney ? instMoney : rewardMoney);
}

int jiaZhengWageFunc(const Servant&serObj)
{
    int instMoney = serObj.getInstMoney();
    int rewardMoney = serObj.getRewardMoney();
    return instMoney*4 + rewardMoney*2;
}
