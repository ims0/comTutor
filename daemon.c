/*********************************************************
    > File Name: daemon.c
    > Author: ims
    > Created Time: Sun 25 Jul 2021 06:10:55 PM CST
 *********************************************************/
/* 调试进程相关id ：ps -o pid,ppid,pgid,sid,tty,comm
 * shell里的命令有独立的组id，有相同的 session id
 * fork 的子进程 继承父进程的组id以及 session id
 */
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

/* 设置一把写锁 */
int writeLocknw(int fd, short start, short whence, short len) {
  struct flock lock;
  lock.l_type = F_WRLCK;
  lock.l_start = start;
  lock.l_whence = whence;
  lock.l_len = len;
  lock.l_pid = getpid();
  //非阻塞方式加锁
  return fcntl(fd, F_SETLK, &lock);
}

/* 解锁 */
int unlock(int fd, short start, short whence, short len) {
  struct flock lock;
  lock.l_type = F_UNLCK;
  lock.l_start = start;
  lock.l_whence = whence;
  lock.l_len = len;
  lock.l_pid = getpid();

  return fcntl(fd, F_SETLK, &lock);
}
void SubProcessContinue() {
  pid_t pid;
  if ((pid = fork()) < 0) {
    printf("fork error!");
    exit(1);
  } else if (pid > 0)
    exit(0);
}

/* 使用 open()函数的 O_EXCL标志作为锁的时候，如果exit没有 unlink，则下次无法打开
 *
 */
void checkUnique() {
  int fd;
  const char *lock = "/tmp/daemon-lock";
  if ((fd = open(lock, O_CREAT | O_WRONLY, 0600)) < 0) {
    kill(getpid(), SIGSEGV);
  }

  if (writeLocknw(fd, 0, SEEK_SET, 0) < 0) {
    kill(getpid(), SIGSEGV);
  }
  /* 对于 fcntl() 锁，close就是解锁，等进程结束，系统关闭fd即可。
   *close(fd);
   */
}
int main() {
  // 1,第一次fork是为调用setsid()做准备，父进程是进程组的组长，调用setsid会失败。
  SubProcessContinue();

  // 2,脱离控制终端，成为登录会话组长和进程组长
  setsid();

  // 3,禁止进程重新打开控制终端
  SubProcessContinue(); //第二子进程不再是会话组长
  printf("daemon pid: %d\n", getpid());
  // 4. 重设文件创建掩模
  umask(0);
  // 5. 改变当前工作目录
  chdir("/tmp/"); // 设置工作目录为根, tmp for debug

  // 6. 关闭打开的文件描述符
  // getdtablesize返回子进程文件描述符表的项数
  printf("fd num:%d\n", getdtablesize());
  for (int i = 0; i < getdtablesize(); i++)
    close(i);
  /*
   * Attach file descriptors 0, 1, and 2 to /dev/null.
   */
  close(0);
  open("/dev/null", O_RDWR);
  dup2(0, 1);
  dup2(0, 2);
  // 7. 忽略SIGCHLD信号
  signal(SIGCHLD, SIG_IGN);

  // 8,唯一副本
  checkUnique();
  // pause();
  sleep(100);

  return 0;
}
