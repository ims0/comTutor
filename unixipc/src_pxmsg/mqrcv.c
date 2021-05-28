#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "mqcom.h"

// mqd_t mq_open(const char *name, int oflag, mode_t mode, struct mq_attr
// *attr); int mq_send(mqd_t mqdes, const char *msg_ptr, size_t msg_len,
// unsigned int msg_prio);
/*
struct mq_attr {
    long mq_flags;   // Message queue flags.
    long mq_maxmsg;  // Maximum number of messages.
    long mq_msgsize; // Maximum message size.
    long mq_curmsgs; // Number of messages currently queued.
};
int mq_getattr(mqd_t mqdes, struct mq_attr *attr);
int mq_setattr(mqd_t mqdes, const struct mq_attr *newattr, struct mq_attr
*oldattr);
ssize_t mq_receive(mqd_t mqdes, char *msg_ptr, size_t msg_len, unsigned int
*msg_prio);
*/
int main() {

  int dummy = 0;
  struct mq_attr oldattr = {0, 0, 0, 0, {dummy}};

  mqd_t mqdes = mq_open(MQ_PATH, O_RDONLY, S_IRUSR, NULL);
  if (mqdes == -1) {
    perror("open Error");
    return -1;
  }

  mq_getattr(mqdes, &oldattr);
  printf("mq_attr.mq_maxmsg:%ld\n", oldattr.mq_maxmsg);
  printf("mq_attr.mq_msgsize:%ld\n", oldattr.mq_msgsize);
  printf("mq_attr.mq_curmsgs:%ld\n", oldattr.mq_curmsgs);

  struct mq_attr newattr;
  memcpy(&newattr, &oldattr, sizeof(struct mq_attr));
  mq_setattr(mqdes, &newattr, &oldattr);

  char msg[MAX_MSG_SIZE]; //>=最大消息大小，不然报错
  unsigned int msg_prio = 0;
  int ret = mq_receive(mqdes, msg, 100, &msg_prio);
  if (ret == -1) {
    perror("mq_receive error");
    return -1;
  } else {
    printf("msg_prio:%u\n", msg_prio);
    printf("rcv:%s\n", msg);
  }
  return 0;
}
