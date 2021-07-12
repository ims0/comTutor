#include <stdio.h>
#include <fstream>
#include <iostream>
#include <regex>
#include "dns_lookup.h"
#include "netinet/ip.h"
#include "arpa/inet.h"
using namespace std;

int main(int argc, char *argv[])
{
    if (argc < 2) {
        fprintf(stderr, "%s <domain> [dnsServer]\n", argv[1]);
        exit(EXIT_FAILURE);
    }
    unsigned addr = 0;
    if( argc == 2 )
    {
        std::ifstream ifs ("/etc/resolv.conf");
        if(!ifs){
            cout<<"open file fail!"<<endl;
            exit(EXIT_FAILURE);
        }
        string line;
        while( !addr && getline(ifs,line) )
        {
            if( line.find("nameserver") !=std::string::npos)
            {
                std::smatch mat;
                std::regex reg("\\d{1,3}(\\.\\d{1,3}){3}");
                std::regex_search (line,mat,reg);

                for (string iter:mat) 
                {
                    std::cout << iter << " " << std::endl;
                    addr = inet_addr(iter.c_str());
                    break;
                }
            }
        }
    }

    char *domainArg = argv[1];
    std::vector<unsigned> veculIPList;
    std::vector<std::string> vecCNameList;
    unsigned ulTimeSpent = 0;
    CDNSLookup dnslookup;
    bool bRet = dnslookup.DNSLookup(addr, domainArg, &veculIPList, &vecCNameList, 1000, &ulTimeSpent);

    printf("DNSLookup result (%s):\n", domainArg);
    if (!bRet)
    {
        printf("timeout!\n");
        return -1;
    }

    for (size_t i = 0; i != veculIPList.size(); ++i)
    {
        printf("IP%lu(unsigned) = %u\n", i + 1, veculIPList[i]);
    }

    for (size_t i = 0; i != vecCNameList.size(); ++i)
    {
        printf("CName%lu = %s\n", i + 1, vecCNameList[i].c_str());
    }
    printf("time spent = %ums\n", ulTimeSpent);

    return 0;
}
