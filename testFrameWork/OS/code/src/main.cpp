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

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        cout << "usage: " << argv[0] << " ip port" << endl;
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

    int iSockFd = socket(AF_INET, SOCK_STREAM, 0);
    if (iSockFd < 0)
    {
        cerr << "fail to create socket, err: " << strerror(errno) << endl;
        return -1;
    }
    cout << "create socket fd " << iSockFd << endl;

    sockaddr_in oAddr;
    memset(&oAddr, 0, sizeof(oAddr));
    oAddr.sin_family = AF_INET;
    oAddr.sin_addr.s_addr =htonl( INADDR_ANY);
    oAddr.sin_port = htons(usPort);
    if (bind(iSockFd, (sockaddr *)&oAddr, sizeof(oAddr)) < 0)
    {
        cerr << "fail to bind addr " << "" << ":" << usPort << ", err: " << strerror(errno) << endl;
        return -1;
    }
    cout << "bind addr " <<"" << ":" << usPort << endl;

    if (listen(iSockFd, 100) < 0)
    {
        cerr << "fail to listen on " << "" << ":" << usPort << ", err: " << strerror(errno) << endl;
    }
    cout << "listen on socket fd " << iSockFd << endl;
    
    int iEpollFd = epoll_create(1024);
    if (iEpollFd < 0)
    {
        cerr << "fail to create epoll, err: " << strerror(errno) << endl;
        return -1;
    }

    epoll_event oEvent;
    oEvent.events = EPOLLIN;
    oEvent.data.fd = iSockFd;
    if (epoll_ctl(iEpollFd, EPOLL_CTL_ADD, iSockFd, &oEvent) < 0)
    {
        cerr << "fail to add listen fd to epoll, err: " << strerror(errno) << endl;
        return -1;
    }

    epoll_event aoEvents[1024];
    uint8_t acRecvBuf[1024 * 1024];
    while (true)
    {
        int iFdCnt = epoll_wait(iEpollFd, aoEvents, 1024, -1);
        if (iFdCnt < 0)
        {
            cerr << "epoll wait error, err: " << strerror(errno) << endl;
            return -1;
        }

        for (int i = 0; i < iFdCnt; i++)
        {
            if (aoEvents[i].data.fd == iSockFd)
            {
                sockaddr_in oClientAddr;
                socklen_t iAddrLen = sizeof(oClientAddr);
                int iAcceptFd = accept(iSockFd, (sockaddr *)&oClientAddr, &iAddrLen);
                if (iAcceptFd < 0)
                {
                    cerr << "fail to accpet, err: " << strerror(errno) << endl;
                    continue;
                }
                cout << "recv connection from " << inet_ntoa(oClientAddr.sin_addr) << ":" << ntohs(oClientAddr.sin_port) << endl;

                oEvent.events = EPOLLIN;
                oEvent.data.fd = iAcceptFd;
                if (epoll_ctl(iEpollFd, EPOLL_CTL_ADD, iAcceptFd, &oEvent) < 0)
                {
                    close(iAcceptFd);
                    cerr << "fail to add fd to epoll, err: " << strerror(errno) << endl;
                    continue;
                }
            }
            else
            {
                int iCurFd = aoEvents[i].data.fd;
                ssize_t iRecvLen = recv(iCurFd, acRecvBuf, sizeof(acRecvBuf), 0);
                if (iRecvLen < 0)
                {
                    cerr << "fail to recv, close connection, err: " << strerror(errno) << endl;
                    if (epoll_ctl(iEpollFd, EPOLL_CTL_DEL, iCurFd, NULL) < 0)
                    {
                        cerr << "fail to del fd from epoll, err: " << strerror(errno) << endl;
                    }
                    close(iCurFd);
                    continue;
                }
                if (iRecvLen == 0)
                {
                    cout << "connection closed by client" << endl;
                    if (epoll_ctl(iEpollFd, EPOLL_CTL_DEL, iCurFd, NULL) < 0)
                    {
                        cerr << "fail to del fd from epoll, err: " << strerror(errno) << endl;
                    }
                    close(iCurFd);
                    continue;
                }
                cout << "recv data len: " << iRecvLen << endl;

                ssize_t iSendLen = send(iCurFd, acRecvBuf, iRecvLen, 0);
                if (iSendLen < 0)
                {
                    cerr << "fail to send, err: " << strerror(errno) << endl;
                    if (epoll_ctl(iEpollFd, EPOLL_CTL_DEL, iCurFd, NULL) < 0)
                    {
                        cerr << "fail to del fd from epoll, err: " << strerror(errno) << endl;
                    }
                    close(iCurFd);
                    break;
                }
                cout << "echo to client, len: " << iSendLen << endl;
            }
        }
    }
}
