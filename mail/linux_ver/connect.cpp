#include <arpa/inet.h>
#include <cerrno>
#include <cstring>
#include <fcntl.h>
#include <netdb.h>
#include <sys/select.h>

#define SOCKET_ERROR (-1)

inline int socket_set_block(int s) {
#ifdef _MSC_VER
  u_long mode = 0;
  return ioctlsocket(s, FIONBIO, &mode);
#else
  int mode = fcntl(s, F_GETFL, 0);
  if (mode == SOCKET_ERROR)
    return SOCKET_ERROR;
  if (mode & O_NONBLOCK)
    return fcntl(s, F_SETFL, mode & ~O_NONBLOCK);
  return 0;
#endif
}
inline int socket_set_nonblock(int s) {
#ifdef _MSC_VER
  u_long mode = 1;
  return ioctlsocket(s, FIONBIO, &mode);
#else
  int mode = fcntl(s, F_GETFL, 0);
  if (mode == SOCKET_ERROR)
    return SOCKET_ERROR;
  if (mode & O_NONBLOCK)
    return 0;
  return fcntl(s, F_SETFL, mode | O_NONBLOCK);
#endif
}

int nb_connect(const char *host, int port) {
  int sock;
  struct sockaddr_in rsock;
  struct hostent *hostinfo;
  struct in_addr *addp;
  if ((hostinfo = gethostbyname(host)) == NULL) {
    return 0;
  }
  //步骤一:socket
  sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock == -1) {
    return 0;
  }
  //步骤二:填充
  memset((char *)&rsock, 0, sizeof(rsock));
  addp = (struct in_addr *)(hostinfo->h_addr_list[0]);
  rsock.sin_addr = *addp;
  rsock.sin_family = AF_INET;
  rsock.sin_port = htons(port);
  int error = -1, slen = sizeof(int);

  timeval tm;
  fd_set wset;

  //步骤三:connect，此时socket设置为非阻塞，connect调用后，无论连接是否建立立即返回-1，
  socket_set_nonblock(sock);

  if (connect(sock, (struct sockaddr *)(&rsock), sizeof(rsock)) == -1) {
    //表示此时tcp三次握手仍旧进行，如果errno不是EINPROGRESS，则说明连接错误，程序结束
    if (errno != EINPROGRESS) {
      return 0;
    } else {
      tm.tv_sec = 5;
      tm.tv_usec = 0;
      FD_ZERO(&wset);
      FD_SET(sock, &wset);
      //监听写集合
      if (select(sock + 1, NULL, &wset, NULL, &tm) > 0) {
        //调用 getsockopt(sockfd,SOL_SOCKET,SO_ERROR,&error,&len);
        //函数返回值来判断是否发生错误 error返回0则表示连接成功！
        getsockopt(sock, SOL_SOCKET, SO_ERROR, &error, (socklen_t *)&slen);
        if (error == 0) {
          socket_set_block(sock);
          return sock;
        } else {
          return 0;
        }
      } else {
        return 0;
      }
    }
  } else {
    socket_set_block(sock);
    return sock;
  }
}
