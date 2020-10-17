#include "unpipc.h"

int main(int argc, char **argv) {
  int i, id;
  struct shmid_ds buff;
  unsigned char c, *ptr;

  if (argc != 2)
    err_quit("usage: shmread <pathname>");

  id = Shmget(Ftok(argv[1], 0), 0, SVSHM_MODE);
  ptr = Shmat(id, NULL, 0);
  Shmctl(id, IPC_STAT, &buff);

  printf("get memId:%d, length = %zu\n", id, buff.shm_segsz);
  /* 4check that ptr[0] = 0, ptr[1] = 1, etc. */
  
  for (i = 0; i < buff.shm_segsz; i++)
    if ((c = *ptr++) != (i % 256))
      err_ret("ptr[%d] = %d", i, c);
    else
      puts("ok");

  exit(0);
}
