#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>

#define SVSHM_MODE (SHM_R | SHM_W | SHM_R >> 3 | SHM_R >> 6)
int main(int argc, char **argv) {
  struct shmid_ds ds_buff;

  if (argc != 3) {
    puts("usage: arg: <pathname> <length>");
    exit(1);
  }
  int length = atoi(argv[2]);
  // IPC_EXCL
  int memid = shmget(ftok(argv[1], 0), length, SVSHM_MODE | IPC_CREAT);
  printf("get memId:%d, length = %d\n", memid, length);
  char *ptr = (char *)shmat(memid, NULL, 0);
  if (ptr == NULL) {
    puts("ptr == NULL");
    exit(1);
  }

  shmctl(memid, IPC_STAT, &ds_buff);

  /* 4check that ptr[0] = 0, ptr[1] = 1, etc. */

  printf("ds_buff.shm_segsz = %zu\nds_buff.shm_nattch = %zu\nds_buff.shm_cpid = %d\n",
         ds_buff.shm_segsz, ds_buff.shm_nattch, ds_buff.shm_cpid);
  for (size_t i = 0; i < ds_buff.shm_segsz; i++)
    *ptr++ = i % 256;

  shmdt(ptr);//not necessary, can be auto remove
  sleep(10);
  shmctl(memid, IPC_RMID, NULL); // delete
  return 0;
}
