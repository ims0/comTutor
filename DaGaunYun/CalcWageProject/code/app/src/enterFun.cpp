
#include<enterFun.h>
#include<map>
#include<cstring>

using std::map;
using std::shared_ptr;
using std::make_shared;

map<string,shared_ptr<Servant>>servantMap;
map<string,shared_ptr<Host>>hostMap;

void InitMap()
{
    servantMap["晴雯"] = make_shared<QingWen>();
    servantMap["麝月"] = make_shared<SheYue>();
    servantMap["袭人"] = make_shared<XiRen>();
    
    hostMap["宝玉"] = make_shared<BaoYu>();
    hostMap["贾母"] = make_shared<JiaMu>();
    hostMap["贾政"] = make_shared<JiaZheng>();
}
// generation one`
int CalcWage(string servantName, string quarterStr)
{
    InitMap();
    if(servantMap.find(servantName) != servantMap.end())
    {

        int quarter = stoi(quarterStr);
        int wage =servantMap[servantName]->CalcServantWage(quarter);
        return wage;
    }
    else
        return 1;
}
// generation two`
int CalcWage(string servantName, string quarterStr, string hostName)
{
    InitMap();
    if(servantMap.find(servantName) != servantMap.end()&&
            hostMap.find(hostName) != hostMap.end())
    {
        int quarter = stoi(quarterStr);
        int startMonth = (quarter - 1)*3 +1;
        int wage = 0;
        for( int month=startMonth ; month < startMonth+3  ; ++month )
        {
            wage+=hostMap[hostName]->hostPayWage(servantMap[servantName], quarter, month);
        }
        return wage;
    }
    else
        return 1;
}
// generation three`
int CalcWage(string servantName, string quarterStr,
        const vector<string>&hostName)
{
    InitMap();
    int quarter = stoi(quarterStr);
    int startMonth = (quarter - 1)*3 +1;
    int wage = 0;
    int month=startMonth;
    for(vector<string>::const_iterator iter=hostName.begin(); iter != hostName.end(); ++iter,++month)
    {
        if(servantMap.find(servantName) != servantMap.end()&&
                hostMap.find(*iter) != hostMap.end())
        {
            wage+=hostMap[*iter]->hostPayWage(servantMap[servantName], quarter, month);
        }
        else
            return 1;
    }
    return wage;
}
int CalcWage(string servantName, string quarterStr,
        const vector<string>&hostName ,const vector<string>&score)
{
    InitMap();
    int aveScore = 0;
    int cnt = 0;
    for(vector<string>::const_iterator iter=score.begin(); iter != score.end(); ++iter)
    {
        aveScore += stoi(*iter);
        cnt++;
    }
    if(cnt == 3)
    {
        aveScore = aveScore/3;
    }
    else
    {
        return 1;
    }
    int addWage = 0;
    if(aveScore >= 8)
        addWage = 1000;
    else if(aveScore < 8 && aveScore >= 6)
        addWage = 500;
    else if(aveScore < 6)
        addWage = 300;
    else 
        return 1;


    int quarter = stoi(quarterStr);
    int startMonth = (quarter - 1)*3 +1;
    int wage = 0;
    int month=startMonth;
    for(vector<string>::const_iterator iter=hostName.begin(); iter != hostName.end(); ++iter,++month)
    {
        if(servantMap.find(servantName) != servantMap.end()&&
                hostMap.find(*iter) != hostMap.end())
        {
            wage+=hostMap[*iter]->hostPayWage(servantMap[servantName], quarter, month);
        }
        else
            return 1;
    }
    return wage + addWage;
}
