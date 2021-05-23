#include "mqcom.h"
#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <limits.h>

// mqd_t mq_open(const char *name, int oflag, mode_t mode, struct mq_attr
// *attr); int mq_send(mqd_t mqdes, const char *msg_ptr, size_t msg_len,
// unsigned int msg_prio);
/*
struct mq_attr {
    long mq_flags;   // Message queue flags.
    long mq_maxmsg;  // Maximum number of messages.
    long mq_msgsize; // Maximum message size.
    long mq_curmsgs; // Number of messages currently queued.
};*/
int main() {

  struct mq_attr attr = {
      .mq_maxmsg = MAX_MSG_NUM,  // default max 10
      .mq_msgsize = MAX_MSG_SIZE // default max 8192
  };

  // 非阻塞可以在创建之后指定也可以通过  mq_setattr 修改
  // 创建的时候如果不指定可读，读的程序也无法以读的形式打开
  mqd_t mqdes =
      mq_open(MQ_PATH, O_WRONLY | O_CREAT , S_IWUSR|S_IRUSR, &attr);
  if (mqdes == -1) {
    perror("open Error");
    return -1;
  }
  mq_getattr(mqdes, &attr);
  printf("mq_attr.mq_flag:%ld\n", attr.mq_flags);
  printf("mq_attr.mq_maxmsg:%ld\n", attr.mq_maxmsg);
  printf("mq_attr.mq_msgsize:%ld\n", attr.mq_msgsize);
  printf("mq_attr.mq_curmsgs:%ld\n", attr.mq_curmsgs);
  attr.mq_flags |= O_NONBLOCK;
  printf("O_NONBLOCK:%d\n", O_NONBLOCK);
  printf("MQ_PRIO_MAX:%d\n", MQ_PRIO_MAX);
  printf("mq_attr.mq_flag:%ld\n", attr.mq_flags);
  mq_setattr(mqdes, &attr, NULL);

  const char *msg = "hello world!";
  unsigned int msg_prio = 1; // <MQ_PRIO_MAX
  int ret = mq_send(mqdes, msg, strlen(msg), msg_prio);
  if (ret == -1) {
    perror("mq_send error,");
    return -1;
  }
  mq_getattr(mqdes, &attr);
  printf("mq_attr.mq_maxmsg:%ld\n", attr.mq_maxmsg);
  printf("mq_attr.mq_msgsize:%ld\n", attr.mq_msgsize);
  printf("mq_attr.mq_curmsgs:%ld\n", attr.mq_curmsgs);
  puts("send succ");
  mq_close(mqdes); //关闭队列指针，程序结束会自动关闭
  return 0;
}
