#include <mqueue.h>
#include <pthread.h>
#include <signal.h> //ubuntu:SIGEV_THREAD
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define handle_error(msg)                                                      \
  do {                                                                         \
    perror(msg);                                                               \
    exit(EXIT_FAILURE);                                                        \
  } while (0)

static void /* Thread start function */
tfunc(union sigval sv) {
  struct mq_attr attr;
  mqd_t mqdes = *((mqd_t *)sv.sival_ptr);

  /* Determine max. msg size; allocate buffer to receive msg */

  if (mq_getattr(mqdes, &attr) == -1)
    handle_error("mq_getattr");
  void *buf = malloc(attr.mq_msgsize);
  if (buf == NULL)
    handle_error("malloc");

  ssize_t nr = mq_receive(mqdes, (char *)buf, attr.mq_msgsize, NULL);
  if (nr == -1)
    handle_error("mq_receive");

  printf("Read %zd bytes from MQ\n", nr);
  free(buf);
  exit(EXIT_SUCCESS); /* Terminate the process */
}

int main(int argc, char *argv[]) {
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

  sev.sigev_notify = SIGEV_THREAD;
  sev.sigev_notify_function = tfunc;
  sev.sigev_notify_attributes = NULL;
  sev.sigev_value.sival_ptr = &mqdes; /* Arg. to thread func. */
  if (mq_notify(mqdes, &sev) == -1)
    handle_error("mq_notify");
  pause();    /* Process will be terminated by thread function */
  return 0;
}
