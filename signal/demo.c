/*********************************************************
  > File Name: demo.c
  > Author: ims
  > Created Time: Thu May 13 12:58:24 2021
 *********************************************************/

#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

void handler(int sig) { printf("recv a sig =%d\n", sig); }

int main() {
  if (signal(SIGALRM, handler) == SIG_ERR) {
    puts("sig fail!");
    return 1;
  }
  while (1) {
    alarm(1);
    sleep(1);
  }
  return 0;
}
