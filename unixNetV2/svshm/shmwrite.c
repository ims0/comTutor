#include "unpipc.h"

int main(int argc, char **argv) {

  if (argc != 2)
    err_quit("usage: shmwrite <pathname>");

  int id = Shmget(Ftok(argv[1], 0), 0, SVSHM_MODE);
  unsigned char *ptr = (unsigned char *)Shmat(id, NULL, 0);

  struct shmid_ds ds_buff;
  Shmctl(id, IPC_STAT, &ds_buff);
  printf("id:%d\ns_buff.shm_segsz = %zu\nds_buff.shm_nattch = "
         "%zu\nds_buff.shm_cpid = %d\n",
         id, ds_buff.shm_segsz, ds_buff.shm_nattch, ds_buff.shm_cpid);

  /* 4set: ptr[0] = 0, ptr[1] = 1, etc. */
  for (size_t i = 0; i < ds_buff.shm_segsz; i++)
    *ptr++ = i % 256;
}
