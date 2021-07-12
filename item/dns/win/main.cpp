#include <stdio.h>
#include "dns_loopup.h"

int main(void)
{
    char szDomainName[] = "www.baidu.com";
    std::vector<ULONG> veculIPList;
    std::vector<std::string> vecstrIPList;
    std::vector<std::string> vecCNameList;
    ULONG ulTimeSpent = 0;
    CDNSLookup dnslookup;
    BOOL bRet = dnslookup.DNSLookup(inet_addr("114.114.114.114"), szDomainName, &vecstrIPList, &vecCNameList, 1000, &ulTimeSpent);

    printf("DNSLookup result (%s):\n", szDomainName);
    if (!bRet)
    {
        printf("timeout!\n");
        return -1;
    }

    for (int i = 0; i != veculIPList.size(); ++i)
    {
        printf("IP%d(ULONG) = %u\n", i + 1, veculIPList[i]);
    }
    for (int i = 0; i != vecstrIPList.size(); ++i)
    {
        printf("IP%d(string) = %s\n", i + 1, vecstrIPList[i].c_str());
    }
    for (int i = 0; i != vecCNameList.size(); ++i)
    {
        printf("CName%d = %s\n", i + 1, vecCNameList[i].c_str());
    }
    printf("time spent = %ums\n", ulTimeSpent);

    return 0;
}
