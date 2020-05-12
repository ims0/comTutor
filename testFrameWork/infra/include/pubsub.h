#ifndef INFRA_INCLUDE_PUBSUB_H
#define INFRA_INCLUDE_PUBSUB_H

#include <netinet/in.h>

#ifdef __cplusplus
extern "C"{
#endif

typedef struct
{
    uint32_t version;
    uint16_t msgId;
    uint16_t priority;
    char *sessionType;
    char *sessionInst;
}PSMsg;

extern int clientSocketFd;
int getMsg(void *buff, size_t len);
int op_pub(PSMsg *psBrief, const char *data, const size_t dataLen);
int  op_sub(const char* sessionType, const char* sessionInst, const size_t pno);

#ifdef __cplusplus
}
#endif
 
#endif
