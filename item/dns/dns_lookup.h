/* 
 * src: https://www.cnblogs.com/snser/p/4101729.html
 * dns protocol define in rcf1035, https://www.ietf.org/rfc/rfc1035.txt.pdf
 */

#include <string>
#include <vector>
#include <netinet/in.h>

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
    unsigned short usTransID; //标识符
    unsigned short usFlags; //各种标志位
    unsigned short usQuestionCount; //Question字段个数
    unsigned short usAnswerCount; //Answer字段个数
    unsigned short usAuthorityCount; //Authority字段个数
    unsigned short usAdditionalCount; //Additional字段个数
};

class CDNSLookup
{
public:
    CDNSLookup();
    ~CDNSLookup();

    bool DNSLookup(unsigned ulDNSServerIP, char *szDomainName, std::vector<unsigned> *pveculIPList = NULL, std::vector<std::string> *pvecstrCNameList = NULL, unsigned ulTimeout = 1000, unsigned *pulTimeSpent = NULL);

private:
    bool DNSLookupCore(unsigned ulDNSServerIP, char *szDomainName, std::vector<unsigned> *pveculIPList, std::vector<std::string> *pvecstrCNameList, unsigned ulTimeout, unsigned *pulTimeSpent);
    bool SendDNSRequest(sockaddr_in sockAddrDNSServer, char *szDomainName);
    bool RecvDNSResponse(sockaddr_in sockAddrDNSServer, unsigned ulTimeout, std::vector<unsigned> *pveculIPList, std::vector<std::string> *pvecstrCNameList, unsigned *pulTimeSpent);
    bool EncodeDotStr(char *szDotStr, char *szEncodedStr, unsigned short nEncodedStrSize);
    bool DecodeDotStr(char *szEncodedStr, unsigned short *pusEncodedStrLen, char *szDotStr, unsigned short nDotStrSize, char *szPacketStartPos = NULL);
    unsigned GetTickCountCalibrate();

private:
    bool m_bIsInitOK;
    int m_sock;
    int m_event;
    unsigned short m_usCurrentProcID;
    char *m_szDNSPacket;
}; 
