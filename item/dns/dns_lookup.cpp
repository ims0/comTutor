#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "dns_lookup.h"

CDNSLookup::CDNSLookup() : m_bIsInitOK(false), m_sock(0), m_szDNSPacket(NULL)
{
    if ((m_sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
    {
        goto init_fail;
    }
    m_szDNSPacket = new (std::nothrow) char[DNS_PACKET_MAX_SIZE];
    if (m_szDNSPacket == NULL)
    {
        goto init_fail;
    }
    m_usCurrentProcID = getpid()&0xffff;
    m_bIsInitOK = true;
    return;
init_fail:
    m_bIsInitOK = false;
}

CDNSLookup::~CDNSLookup()
{
    if (m_szDNSPacket != NULL)
    {
        delete [] m_szDNSPacket;
    }
}

void print_recv_ip(in_addr_t dest_addr)
{
    printf("recv PING (%d.%d.%d.%d) data.\n",
            (dest_addr&0x000000ff), (dest_addr&0x0000ff00)>>8,
            (dest_addr&0x00ff0000)>>16, (dest_addr&0xff000000)>>24);
}
bool CDNSLookup::DNSLookup(unsigned DNSServerIP, char *szDomainName, std::vector<unsigned> *pveculIPList, std::vector<std::string> *pvecstrCNameList, unsigned ulTimeout, unsigned *pulTimeSpent)
{
    return DNSLookupCore(DNSServerIP, szDomainName, pveculIPList, pvecstrCNameList, ulTimeout, pulTimeSpent);
}


bool CDNSLookup::DNSLookupCore(unsigned DNSServerIP, char *szDomainName, std::vector<unsigned> *pveculIPList, std::vector<std::string> *pvecstrCNameList, unsigned ulTimeout, unsigned *pulTimeSpent)
{
    if (m_bIsInitOK == false || szDomainName == NULL)
    {
        return false;
    }

    //配置SOCKET
    sockaddr_in sockAddrDNSServer;
    sockAddrDNSServer.sin_family = AF_INET;
    sockAddrDNSServer.sin_addr.s_addr = DNSServerIP;
    sockAddrDNSServer.sin_port = htons( DNS_PORT );

    //DNS查询与解析
    if (!SendDNSRequest(sockAddrDNSServer, szDomainName)
        || !RecvDNSResponse(sockAddrDNSServer, ulTimeout, pveculIPList, pvecstrCNameList, pulTimeSpent))
    {
        return false;
    }

    return true;
}

bool CDNSLookup::SendDNSRequest(sockaddr_in sockAddrDNSServer, char *szDomainName)
{
    char *pWriteDNSPacket = m_szDNSPacket;
    memset(pWriteDNSPacket, 0, DNS_PACKET_MAX_SIZE);

    //填充DNS查询报文头部
    DNSHeader *pDNSHeader = (DNSHeader*)pWriteDNSPacket;
    pDNSHeader->usTransID = m_usCurrentProcID;
    pDNSHeader->usFlags = htons(0x0100);
    pDNSHeader->usQuestionCount = htons(0x0001);
    pDNSHeader->usAnswerCount = 0x0000;
    pDNSHeader->usAuthorityCount = 0x0000;
    pDNSHeader->usAdditionalCount = 0x0000;

    //设置DNS查询报文内容
    unsigned short usQType = htons(0x0001);
    unsigned short usQClass = htons(0x0001);
    unsigned short nDomainNameLen = strlen(szDomainName);
    char *szEncodedDomainName = (char *)malloc(nDomainNameLen + 2);
    if (szEncodedDomainName == NULL)
    {
        return false;
    }
    if (!EncodeDotStr(szDomainName, szEncodedDomainName, nDomainNameLen + 2))
    {
        return false;
    }

    //填充DNS查询报文内容
    unsigned short nEncodedDomainNameLen = strlen(szEncodedDomainName) + 1;
    memcpy(pWriteDNSPacket += sizeof(DNSHeader), szEncodedDomainName, nEncodedDomainNameLen);
    memcpy(pWriteDNSPacket += nEncodedDomainNameLen, (char*)(&usQType), DNS_TYPE_SIZE);
    memcpy(pWriteDNSPacket += DNS_TYPE_SIZE, (char*)(&usQClass), DNS_CLASS_SIZE);
    free (szEncodedDomainName);

    //发送DNS查询报文
    unsigned short nDNSPacketSize = sizeof(DNSHeader) + nEncodedDomainNameLen + DNS_TYPE_SIZE + DNS_CLASS_SIZE;
    if (sendto(m_sock, m_szDNSPacket, nDNSPacketSize, 0, (sockaddr*)&sockAddrDNSServer, sizeof(sockAddrDNSServer)) == -1)
    {
        return false;
    }

    return true;
}

bool CDNSLookup::RecvDNSResponse(sockaddr_in sockAddrDNSServer, unsigned ulTimeout, std::vector<unsigned> *pveculIPList, std::vector<std::string> *pvecstrCNameList, unsigned *pulTimeSpent)
{
    unsigned ulSendTimestamp = GetTickCountCalibrate();

    if (pveculIPList != NULL)
    {
        pveculIPList->clear();
    }
    if (pvecstrCNameList != NULL)
    {
        pvecstrCNameList->clear();
    }

    char recvbuf[1024] = {'\0'};
    char szDotName[128] = {'\0'};
    unsigned short nEncodedNameLen = 0;
    fd_set read_fd;
    struct timeval tv = {.tv_sec = 1, .tv_usec = 20000};
    int ret;
    socklen_t len = sizeof(struct sockaddr_in);
    while (true)
    {
        FD_ZERO(&read_fd);
        FD_SET(m_sock, &read_fd);
        ret = select(m_sock+1, &read_fd, NULL, NULL, NULL);
        //if (WSAWaitForMultipleEvents(1, &m_event, false, 100, false) != WSA_WAIT_TIMEOUT)
        printf("select:%d\n",ret);
        switch(ret)
        {
            case -1:
                fprintf(stderr,"fail to select!\n");
                break;
            case 0:
                break;
            default:
            {
                unsigned ulRecvTimestamp = GetTickCountCalibrate();

                fprintf(stderr,"recv msg!\n");
                //接收响应报文
                if (recvfrom(m_sock, recvbuf, 1024, 0, (struct sockaddr*)&sockAddrDNSServer, &len) != -1)
                {
                    fprintf(stderr,"proc msg!\n");
                    DNSHeader *pDNSHeader = (DNSHeader*)recvbuf;
                    unsigned short usQuestionCount = 0;
                    unsigned short usAnswerCount = 0;

                    if (pDNSHeader->usTransID == m_usCurrentProcID
                        && (ntohs(pDNSHeader->usFlags) & 0xfb7f) == 0x8100 //RFC1035 4.1.1(Header section format)
                        && (usQuestionCount = ntohs(pDNSHeader->usQuestionCount)) >= 0
                        && (usAnswerCount = ntohs(pDNSHeader->usAnswerCount)) > 0)
                    {
                        char *pDNSData = recvbuf + sizeof(DNSHeader);

                        //解析Question字段
                        for (int q = 0; q != usQuestionCount; ++q)
                        {
                            if (!DecodeDotStr(pDNSData, &nEncodedNameLen, szDotName, sizeof(szDotName)))
                            {
                                return false;
                            }
                            pDNSData += (nEncodedNameLen + DNS_TYPE_SIZE + DNS_CLASS_SIZE);
                        }

                        //解析Answer字段
                        for (int a = 0; a != usAnswerCount; ++a)
                        {
                            if (!DecodeDotStr(pDNSData, &nEncodedNameLen, szDotName, sizeof(szDotName), recvbuf))
                            {
                                return false;
                            }
                            pDNSData += nEncodedNameLen;

                            unsigned short usAnswerType = ntohs(*(unsigned short*)(pDNSData));
                            //unsigned short usAnswerClass = ntohs(*(unsigned short*)(pDNSData + DNS_TYPE_SIZE));
                            //unsigned usAnswerTTL = ntohl(*(unsigned*)(pDNSData + DNS_TYPE_SIZE + DNS_CLASS_SIZE));
                            unsigned short usAnswerDataLen = ntohs(*(unsigned short*)(pDNSData + DNS_TYPE_SIZE + DNS_CLASS_SIZE + DNS_TTL_SIZE));
                            pDNSData += (DNS_TYPE_SIZE + DNS_CLASS_SIZE + DNS_TTL_SIZE + DNS_DATALEN_SIZE);

                            if (usAnswerType == DNS_TYPE_A && pveculIPList != NULL)
                            {
                                unsigned ulIP = *(unsigned*)(pDNSData);
                                print_recv_ip(ulIP);
                                pveculIPList->push_back(ulIP);
                            }
                            else if (usAnswerType == DNS_TYPE_CNAME && pvecstrCNameList != NULL)
                            {
                                if (!DecodeDotStr(pDNSData, &nEncodedNameLen, szDotName, sizeof(szDotName), recvbuf))
                                {
                                    return false;
                                }
                                pvecstrCNameList->push_back(szDotName);
                            }

                            pDNSData += (usAnswerDataLen);
                        }

                        //计算DNS查询所用时间
                        if (pulTimeSpent != NULL)
                        {
                            *pulTimeSpent = ulRecvTimestamp - ulSendTimestamp;
                        }

                        return 1;
                    }
                }
            }
        }

        //超时退出
        if (GetTickCountCalibrate() - ulSendTimestamp > ulTimeout)
        {
            *pulTimeSpent = ulTimeout + 1;
            return false;
        }
    }

    return true;
}

/*
 * convert "www.baidu.com" to "\x03www\x05baidu\x03com"
 * 0x0000 03 77 77 77 05 62 61 69 64 75 03 63 6f 6d 00 ff
 */
bool CDNSLookup::EncodeDotStr(char *szDotStr, char *szEncodedStr, unsigned short nEncodedStrSize)
{
    unsigned short nDotStrLen = strlen(szDotStr);

    if (szDotStr == NULL || szEncodedStr == NULL || nEncodedStrSize < nDotStrLen + 2)
    {
        return false;
    }

    char *szDotStrCopy = new char[nDotStrLen + 1];
    //strcpy_s(szDotStrCopy, nDotStrLen + 1, szDotStr);
    strcpy(szDotStrCopy, szDotStr);

    //char *pNextToken = NULL;
    //char *pLabel = strtok_s(szDotStrCopy, ".", &pNextToken);
    char *pLabel = strtok(szDotStrCopy, ".");
    unsigned short nLabelLen = 0;
    unsigned short nEncodedStrLen = 0;
    while (pLabel != NULL)
    {
        if ((nLabelLen = strlen(pLabel)) != 0)
        {
            //sprintf_s(szEncodedStr + nEncodedStrLen, nEncodedStrSize - nEncodedStrLen, "%c%s", nLabelLen, pLabel);
            sprintf(szEncodedStr + nEncodedStrLen, "%c%s", nLabelLen, pLabel);
            nEncodedStrLen += (nLabelLen + 1);
        }
        //pLabel = strtok_s(NULL, ".", &pNextToken);
        pLabel = strtok(NULL, ".");
    }

    delete [] szDotStrCopy;

    return true;
}

/*
 * convert "\x03www\x05baidu\x03com\x00" to "www.baidu.com"
 * 0x0000 03 77 77 77 05 62 61 69 64 75 03 63 6f 6d 00 ff
 * convert "\x03www\x05baidu\xc0\x13" to "www.baidu.com"
 * 0x0000 03 77 77 77 05 62 61 69 64 75 c0 13 ff ff ff ff
 * 0x0010 ff ff ff 03 63 6f 6d 00 ff ff ff ff ff ff ff ff
 */
bool CDNSLookup::DecodeDotStr(char *szEncodedStr, unsigned short *pusEncodedStrLen, char *szDotStr, unsigned short nDotStrSize, char *szPacketStartPos)
{
    if (szEncodedStr == NULL || pusEncodedStrLen == NULL || szDotStr == NULL)
    {
        return false;
    }

    char *pDecodePos = szEncodedStr;
    unsigned short usPlainStrLen = 0;
    char nLabelDataLen = 0;
    *pusEncodedStrLen = 0;

    while ((nLabelDataLen = *pDecodePos) != 0x00)
    {
        if ((nLabelDataLen & 0xc0) == 0) //普通格式，LabelDataLen + Label
        {
            if (usPlainStrLen + nLabelDataLen + 1 > nDotStrSize)
            {
                return false;
            }
            memcpy(szDotStr + usPlainStrLen, pDecodePos + 1, nLabelDataLen);
            memcpy(szDotStr + usPlainStrLen + nLabelDataLen, ".", 1);
            pDecodePos += (nLabelDataLen + 1);
            usPlainStrLen += (nLabelDataLen + 1);
            *pusEncodedStrLen += (nLabelDataLen + 1);
        }
        else //消息压缩格式，11000000 00000000，两个字节，前2位为跳转标志，后14位为跳转的偏移
        {
            if (szPacketStartPos == NULL)
            {
                return false;
            }
            unsigned short usJumpPos = ntohs(*(unsigned short*)(pDecodePos)) & 0x3fff;
            unsigned short nEncodeStrLen = 0;
            if (!DecodeDotStr(szPacketStartPos + usJumpPos, &nEncodeStrLen, szDotStr + usPlainStrLen, nDotStrSize - usPlainStrLen, szPacketStartPos))
            {
                return false;
            }
            else
            {
                *pusEncodedStrLen += 2;
                return true;
            }
        }
    }

    szDotStr[usPlainStrLen - 1] = '\0';
    *pusEncodedStrLen += 1;

    return true;
}
unsigned CDNSLookup::GetTickCountCalibrate()
{
    /*
    static unsigned s_ulFirstCallTick = 0;
    static long s_ullFirstCallTickMS = 0;

    SYSTEMTIME systemtime;
    FILETIME filetime;
    GetLocalTime(&systemtime);
    SystemTimeToFileTime(&systemtime, &filetime);
    LARGE_INTEGER liCurrentTime;
    liCurrentTime.HighPart = filetime.dwHighDateTime;
    liCurrentTime.LowPart = filetime.dwLowDateTime;
    long llCurrentTimeMS = liCurrentTime.QuadPart / 10000;

    if (s_ulFirstCallTick == 0)
    {
        s_ulFirstCallTick = GetTickCount();
    }
    if (s_ullFirstCallTickMS == 0)
    {
        s_ullFirstCallTickMS = llCurrentTimeMS;
    }

    return s_ulFirstCallTick + (unsigned)(llCurrentTimeMS - s_ullFirstCallTickMS);
    */
    return 1;
}
