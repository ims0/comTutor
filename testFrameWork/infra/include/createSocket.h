#ifndef CREATESOCKET_H
#define CREATESOCKET_H

#include <sys/types.h>
#include <sys/socket.h>

#ifdef __cplusplus
extern "C"{
#endif

int getSocketFd(int port, const char *ipAddr);



#ifdef __cplusplus
}
#endif

#endif
