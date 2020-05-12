#include <cstring>
#include "pubsub.h"


int op_pub(PSMsg *psBrief, const char *data, const size_t dataLen)
{
    char *sendData = new char[sizeof(PSMsg)+dataLen];
    memcpy(sendData,psBrief,sizeof(PSMsg));
    memcpy(sendData+sizeof(PSMsg),data,dataLen);
    send(clientSocketFd, sendData, sizeof(PSMsg)+dataLen, 0);
    delete sendData;
    return 0;
}

int getMsg(void *buff, size_t len)
{
    return recv(clientSocketFd, buff, len,0);
}
int  op_sub(const char* sessionType, const char* sessionInst, const size_t pno)
{
    return 0;
}
