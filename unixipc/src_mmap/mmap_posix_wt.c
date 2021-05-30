/*-------------map_normalfile1.c-----------*/
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
typedef struct {
  char name[4];
  int age;
} people;
int main(int argc, char **argv) // map a normal file as shared mem:
{
  const char *px_name = argv[1];
  if (!px_name) {
    fprintf(stderr, "usage: %s <posix_name>\n", argv[0]);
    return 1;
  }
  // O_TRUNC:若文件存在，则长度被截为0，属性不变
  int oflag = O_RDWR | O_CREAT | O_TRUNC;
  mode_t mode = S_IRUSR | S_IWUSR;
  int fd = shm_open(px_name, oflag, mode);
  if (fd == -1) {
    fprintf(stderr, "shm_open: %s\n", strerror(errno));
    return 1;
  }
  printf("fd: %d \n", fd);
  ftruncate(fd, sizeof(people) * 10);

  people *p_map = (people *)mmap(NULL, sizeof(people) * 10, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  if (p_map == (void *)-1) {
    fprintf(stderr, "mmap: %s\n", strerror(errno));
    return 1;
  }
  close(fd);
  char temp = 'a';
  for (int i = 0; i < 10; i++) {
    temp += 1;
    (*(p_map + i)).name[1] = '\0';
    memcpy((*(p_map + i)).name, &temp, 1);
    (*(p_map + i)).age = 20 + i;
  }
  printf("initializeover\n");
  sleep(10);
  munmap(p_map, sizeof(people) * 10);
  printf("umapok\n");
  return 0;
}
