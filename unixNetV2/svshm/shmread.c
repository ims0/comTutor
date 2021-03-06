#include "unpipc.h"

int main(int argc, char **argv) {
  if (argc != 2)
    err_quit("usage: shmread <pathname>");

  int id = Shmget(Ftok(argv[1], 0), 0, SVSHM_MODE);
  unsigned char *ptr = (unsigned char *)Shmat(id, NULL, 0);
  struct shmid_ds buff;
  Shmctl(id, IPC_STAT, &buff);

  printf("get memId:%d, length = %zu\n", id, buff.shm_segsz);
  /* 4check that ptr[0] = 0, ptr[1] = 1, etc. */

  unsigned char c;
  for (size_t i = 0; i < buff.shm_segsz; i++)
    if ((c = *ptr++) != (i % 256))
      err_ret("ptr[%d] = %d", i, c);
    else
      puts("ok");

  exit(0);
}
