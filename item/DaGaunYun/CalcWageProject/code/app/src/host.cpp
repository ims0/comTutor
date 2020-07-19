#include<host.h>
#include<servant.h>



int BaoYu::hostPayWage(std::shared_ptr<Servant>&servant,int quarter, int month)
{
    return 8 * (servant->getInstMoney(quarter,month)+servant->getRewardMoney(quarter,month));
}

int JiaMu::hostPayWage(std::shared_ptr<Servant>&servant,int quarter, int month)
{
    int instMoney = servant->getInstMoney(quarter,month);
    int rewardMoney = servant->getRewardMoney(quarter,month);

    return 10 * (instMoney > rewardMoney ? instMoney : rewardMoney);
}
int JiaZheng::hostPayWage(std::shared_ptr<Servant>&servant,int quarter, int month)
{
    int instMoney = servant->getInstMoney(quarter,month);
    int rewardMoney = servant->getRewardMoney(quarter,month);

    return  instMoney*4 + rewardMoney*2;
}
