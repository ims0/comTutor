#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <sys/epoll.h>

using namespace std;
const int maxEvents = 20;
const int recvBuffLen = 1024;
int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        cout << "usage: " << argv[0] << " port" << endl;
        return -1;
    }
    /*
    char *szIp = argv[1];
    in_addr_t iIp = inet_addr(szIp);
    if (iIp == INADDR_NONE)
    {
        cerr << "fail to parse ip: " << szIp << endl;
        return -1;
    }*/
    char *pEnd = NULL;
    uint16_t usPort = strtoul(argv[1], &pEnd, 10);
    if (*pEnd != '\0')
    {
        cerr << "fail to parse port: " << argv[1] << endl;
        return -1;
    }

    int serveSockFd = socket(AF_INET, SOCK_STREAM, 0);
    if (serveSockFd < 0)
    {
        cerr << "fail to create socket, err: " << strerror(errno) << endl;
        return -1;
    }
    cout << "create socket fd " << serveSockFd << endl;

    sockaddr_in serverAddrIn;
    memset(&serverAddrIn, 0, sizeof(serverAddrIn));
    serverAddrIn.sin_family = AF_INET;
    /*uint32_t htonl(uint32_t hostlong);*/
    serverAddrIn.sin_addr.s_addr =htonl(INADDR_ANY);
    /*uint16_t htons(uint16_t hostshort);*/
    serverAddrIn.sin_port = htons(usPort);
    cout<<"htons"<<htons(usPort)<<endl;
    if (inet_aton("127.0.0.1", &(serverAddrIn.sin_addr)) != 1)
    {
        fprintf(stderr, "inet_aton error!!!\n");
        exit(1);
    }
    if (bind(serveSockFd, (sockaddr *)&serverAddrIn, sizeof(serverAddrIn)) < 0)
    {
        cerr << "fail to bind addr " << ":" << usPort << ", err: " << strerror(errno) << endl;
        return -1;
    }
    cout << "bind addr " << ":" << usPort << endl;

    if (listen(serveSockFd, 100) < 0)
    {
        cerr << "fail to listen on " << ":" << usPort << ", err: " << strerror(errno) << endl;
    }
    cout << "listen on socket fd " << serveSockFd << endl;

    int instanceOfEpoll = epoll_create(maxEvents);
    if (instanceOfEpoll < 0)
    {
        cerr << "fail to create epoll, err: " << strerror(errno) << endl;
        return -1;
    }

    epoll_event ctlEvent;
    ctlEvent.events = EPOLLIN;// EPOLLIN, The associated file is available for read(2) operations.
    ctlEvent.data.fd = serveSockFd;
    if (epoll_ctl(instanceOfEpoll, EPOLL_CTL_ADD, serveSockFd, &ctlEvent) < 0)
    {
        cerr << "fail to add listen fd to epoll, err: " << strerror(errno) << endl;
        return -1;
    }

    epoll_event waitEvents[maxEvents];
    uint8_t recvBuff[recvBuffLen];
    while (true)
    {
        /* int epoll_wait(int epfd, struct epoll_event *events,
         *                       int maxEvents, int timeout);
         * timeout=0, timeout equal  to  zero  cause epoll_wait() to return immediately
         * timeout=-1, timeout of -1 causes epoll_wait() to block indefinitely */
        int numOfReadyFd = epoll_wait(instanceOfEpoll, waitEvents, maxEvents, -1);
        if (numOfReadyFd < 0)
        {
            cerr << "epoll wait error, err: " << strerror(errno) << endl;
            return -1;
        }

        for (int i = 0; i < numOfReadyFd; i++)
        {
            if (waitEvents[i].data.fd == serveSockFd)
            {
                sockaddr_in clientSockAddr;
                socklen_t iAddrLen = sizeof(clientSockAddr);
                int acceptFd = accept(serveSockFd, (sockaddr *)&clientSockAddr, &iAddrLen);
                if (acceptFd < 0)
                {
                    cerr << "fail to accpet, err: " << strerror(errno) << endl;
                    continue;
                }
                cout << "\nrecv connection from " << inet_ntoa(clientSockAddr.sin_addr) << ":" << ntohs(clientSockAddr.sin_port) << endl;

                ctlEvent.events = EPOLLIN;
                ctlEvent.data.fd = acceptFd;
                if (epoll_ctl(instanceOfEpoll, EPOLL_CTL_ADD, acceptFd, &ctlEvent) < 0)
                {
                    close(acceptFd);
                    cerr << "fail to add fd to epoll, err: " << strerror(errno) << endl;
                    continue;
                }
                else
                    cout<<"add accept fd:"<<acceptFd<< " to EPOLL_CTL"<<endl;
            }
            else
            {
                int readyFd = waitEvents[i].data.fd;
                bzero(recvBuff,recvBuffLen);
                ssize_t recvDataLen = recv(readyFd, recvBuff, sizeof(recvBuff), MSG_DONTROUTE);
                if (recvDataLen < 0)
                {
                    cerr << "fail to recv, close connection, err: " << strerror(errno) << endl;
                    if (epoll_ctl(instanceOfEpoll, EPOLL_CTL_DEL, readyFd, NULL) < 0)
                    {
                        cerr << "fail to del fd from epoll, err: " << strerror(errno) << endl;
                    }
                    close(readyFd);
                    continue;
                }
                if (recvDataLen == 0)
                {
                    cout << "connection closed by client" << endl;
                    if (epoll_ctl(instanceOfEpoll, EPOLL_CTL_DEL, readyFd, NULL) < 0)
                    {
                        cerr << "fail to del fd from epoll, err: " << strerror(errno) << endl;
                    }
                    close(readyFd);
                    continue;
                }
                cout << "recv data:"<< recvBuff << endl;

                ssize_t iSendLen = send(readyFd, recvBuff, recvDataLen, 0);
                if (iSendLen < 0)
                {
                    cerr << "fail to send, err: " << strerror(errno) << endl;
                    if (epoll_ctl(instanceOfEpoll, EPOLL_CTL_DEL, readyFd, NULL) < 0)
                    {
                        cerr << "fail to del fd from epoll, err: " << strerror(errno) << endl;
                    }
                    close(readyFd);
                    break;
                }
                cout << "echo to client, len: " << iSendLen << endl;
            }
        }
    }
}
