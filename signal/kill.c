/*********************************************************
  > File Name: demo.c
  > Author: ims
  > Created Time: Thu May 13 12:58:24 2021
 *********************************************************/

#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
//int kill(pid_t pid, int sig);
void handler(int sig) { printf("recv a sig =%d\n", sig); }

int main() {
  if (signal(SIGALRM, handler) == SIG_ERR) {
    puts("sig fail!");
    return 1;
  }
  while (1) {
    kill(getpid(),SIGSEGV);
    sleep(1);
  }
  return 0;
}
