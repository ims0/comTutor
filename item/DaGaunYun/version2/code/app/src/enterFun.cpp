
#include<enterFun.h>
#include <memory>
#include<map>
#include"servant.h"
#include"wageCalcFunc.h"
using std::map;
using std::shared_ptr;
using std::make_shared;
using std::string;
using std::vector;

map<string,shared_ptr<Servant>>servantMap;
map<string,WageCalcFunc>hostMap;

void InitMap()
{
    servantMap["晴雯"] = make_shared<QingWen>();
    servantMap["麝月"] = make_shared<SheYue>();
    servantMap["袭人"] = make_shared<XiRen>();
    
    hostMap["宝玉"] = baoYuWageFunc;
    hostMap["贾母"] = jiaMuWageFunc;
    hostMap["贾政"] = jiaZhengWageFunc;
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
    if(servantMap.find(servantName) == servantMap.end())
        return 1;
    vector<string>::const_iterator iter;
    for(iter = hostName.begin(); iter != hostName.end(); ++iter,++month)
    {
        if(hostMap.find(*iter) != hostMap.end())
        {
            wage+=servantMap[servantName]->wage(hostMap[*iter], quarter, month);
        }
        else
            return 1;
    }
    return wage;
}

