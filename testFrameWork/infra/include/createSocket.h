#ifndef CREATESOCKET_H
#define CREATESOCKET_H

#ifdef __cplusplus
extern "C"{
#endif

int getSocketFd(int port, const char *ipAddr);

#ifdef __cplusplus
}
#endif

#endif
