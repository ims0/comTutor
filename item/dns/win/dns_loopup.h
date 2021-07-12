#pragma once
// src: https://www.cnblogs.com/snser/p/4101729.html
//
//这里需要导入库 Ws2_32.lib，在不同的IDE下可能不太一样
//#pragma comment(lib, "Ws2_32.lib")

#include <windows.h>
#include <string>
#include <vector>

#define MAX_DOMAINNAME_LEN  255
#define DNS_PORT            53
#define DNS_TYPE_SIZE       2
#define DNS_CLASS_SIZE      2
#define DNS_TTL_SIZE        4
#define DNS_DATALEN_SIZE    2
#define DNS_TYPE_A          0x0001 //1 a host address
#define DNS_TYPE_CNAME      0x0005 //5 the canonical name for an alias
#define DNS_PACKET_MAX_SIZE (sizeof(DNSHeader) + MAX_DOMAINNAME_LEN + DNS_TYPE_SIZE + DNS_CLASS_SIZE)

struct DNSHeader
{
    USHORT usTransID; //标识符
    USHORT usFlags; //各种标志位
    USHORT usQuestionCount; //Question字段个数
    USHORT usAnswerCount; //Answer字段个数
    USHORT usAuthorityCount; //Authority字段个数
    USHORT usAdditionalCount; //Additional字段个数
};

class CDNSLookup
{
public:
    CDNSLookup();
    ~CDNSLookup();

    BOOL DNSLookup(ULONG ulDNSServerIP, char *szDomainName, std::vector<ULONG> *pveculIPList = NULL, std::vector<std::string> *pvecstrCNameList = NULL, ULONG ulTimeout = 1000, ULONG *pulTimeSpent = NULL);
    BOOL DNSLookup(ULONG ulDNSServerIP, char *szDomainName, std::vector<std::string> *pvecstrIPList = NULL, std::vector<std::string> *pvecstrCNameList = NULL, ULONG ulTimeout = 1000, ULONG *pulTimeSpent = NULL);

private:
    BOOL Init();
    BOOL UnInit();
    BOOL DNSLookupCore(ULONG ulDNSServerIP, char *szDomainName, std::vector<ULONG> *pveculIPList, std::vector<std::string> *pvecstrCNameList, ULONG ulTimeout, ULONG *pulTimeSpent);
    BOOL SendDNSRequest(sockaddr_in sockAddrDNSServer, char *szDomainName);
    BOOL RecvDNSResponse(sockaddr_in sockAddrDNSServer, ULONG ulTimeout, std::vector<ULONG> *pveculIPList, std::vector<std::string> *pvecstrCNameList, ULONG *pulTimeSpent);
    BOOL EncodeDotStr(char *szDotStr, char *szEncodedStr, USHORT nEncodedStrSize);
    BOOL DecodeDotStr(char *szEncodedStr, USHORT *pusEncodedStrLen, char *szDotStr, USHORT nDotStrSize, char *szPacketStartPos = NULL);
    ULONG GetTickCountCalibrate();

private:
    BOOL m_bIsInitOK;
    SOCKET m_sock;
    WSAEVENT m_event;
    USHORT m_usCurrentProcID;
    char *m_szDNSPacket;
}; 
