#include <mqueue.h>
#include <pthread.h>
#include <signal.h> //ubuntu:SIGEV_THREAD
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
/*
 *  union sigval {           通知传递的数据 
           int     sival_int;          Integer value 
           void   *sival_ptr;          Pointer value 
       };

   struct sigevent {
       int          sigev_notify;  通知方法 
       int          sigev_signo;   通知信号 
       union sigval sigev_value;   通知传递的数据 
       void       (*sigev_notify_function) (union sigval); 线程通知的函数 (SIGEV_THREAD) 
       void        *sigev_notify_attributes; 线程通知的线程属性 (SIGEV_THREAD) 
       pid_t        sigev_notify_thread_id; 用于接收信号的线程ID (SIGEV_THREAD_ID) 
   };
*/

#define handle_error(msg)                                                      \
  do {                                                                         \
    perror(msg);                                                               \
    exit(EXIT_FAILURE);                                                        \
  } while (0)

void sighandler(int sig)
{
    printf("sig:%d\n", sig);
}

int main(int argc, char *argv[]) {

  int NOTIFY_SIG = SIGUSR1;  // 自定义通知信号的种类,

  /* Type of a signal handler.  */
  //typedef void (*__sighandler_t) (int);
  // __sighandler_t signal (int __sig, __sighandler_t __handler)
  signal(SIGUSR1, sighandler);  // 仅仅用于跳出sigsuspend
  struct sigevent sev;

  if (argc != 2) {
    fprintf(stderr, "Usage: %s <mq-name>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  fprintf(stdout, "mq-name:%s\n", argv[1]);
  mqd_t mqdes = mq_open(argv[1], O_RDONLY, S_IRUSR, NULL);
  //mqdes = mq_open(argv[1], O_RDONLY);
  if (mqdes == (mqd_t)-1)
    handle_error("mq_open");

  sev.sigev_notify = SIGEV_SIGNAL;
  sev.sigev_signo = NOTIFY_SIG;
  if (mq_notify(mqdes, &sev) == -1)
  {
    handle_error("mq_notify");
  }
  sigset_t empty_mask;
  sigemptyset(&empty_mask);
  while (1) {
      sigsuspend(&empty_mask);
      mq_notify(mqdes, &sev);//再次注册
      // TODO: 处理由空变为非空的消息队列
  }

  return 0;
}
