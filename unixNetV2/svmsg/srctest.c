#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#define SVMSG_MODE S_IRUSR | S_IWUSR

void testModeArg() {
  printf("S_IRUSR:%03o\n", S_IRUSR);
  printf("S_IRUSR:%03o\n", S_IRUSR >> 3);
  printf("S_IRUSR:%03o\n", S_IRUSR >> 6);
}

typedef struct {
  long mtype;     /* message type must be > 0 */
  char mtext[10]; /* start of message data*/
} Message;

int main() {
  testModeArg();
  int mqid = msgget(IPC_PRIVATE, SVMSG_MODE | IPC_CREAT);

  int res = 0;
  Message buf, rcvbuf; // msgptr
  buf.mtype = 10;      /* type of received/sent message */
  buf.mtext[0] = 100;
  int msgLen = sizeof(Message) - sizeof(long);
  // int msgsnd(int mqid, const void *msgp, size_t msgsz, int msgflg);
  res = msgsnd(mqid, &buf, msgLen, IPC_NOWAIT); // set no block
  buf.mtype = 20;
  buf.mtext[0] = 120;
  res = msgsnd(mqid, &buf, msgLen, IPC_NOWAIT); // set no block
  perror("msgsnd return");
  struct msqid_ds info;
  msgctl(mqid, IPC_STAT, &info); /* Get `ipc_perm' options.  */

  printf(
      "--- IPC_STAT ---\n"
      "read-write: %03o,\ncurrent number of bytes on queue = %lu,\nnumber of "
      "messages currently on queue = %lu,\nmax number of bytes allowed on "
      "queue = %lu\n",
      info.msg_perm.mode & 0777, info.msg_cbytes, info.msg_qnum,
      info.msg_qbytes);

  res = system("ipcs -q");
  (void)res;
  /* ssize_t msgrcv(int msqid, void *msgp, size_t msgsz, long msgtyp, int
   msgflg);*/
  res = msgrcv(mqid, &rcvbuf, msgLen, 10, SVMSG_MODE); // set no block
  perror("msgrcv return");
  printf("msg type:%ld,data:%d\n", rcvbuf.mtype, rcvbuf.mtext[0]);

  res = msgrcv(mqid, &rcvbuf, msgLen, 20, SVMSG_MODE); // set no block
  perror("msgrcv return");
  printf("msg type:%ld,data:%d\n", rcvbuf.mtype, rcvbuf.mtext[0]);

  res = system("ipcs -q");
  msgctl(mqid, IPC_RMID, NULL); /* Remove identifier.  */

  perror("msgctl IPC_RMID return");
  return 0;
}
