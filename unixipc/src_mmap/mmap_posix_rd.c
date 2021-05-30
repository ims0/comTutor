/*-------------map_normalfile2.c-----------*/
#include <errno.h>
#include <fcntl.h>
#include <fcntl.h> /* For O_* constants */
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h> /* For mode constants */
#include <sys/types.h>
#include <unistd.h>
typedef struct {
  char name[4];
  int age;
} people;
int main(int argc, char **argv) // map a normal file as shared mem:
{
  people *p_map;
  const char *px_name = argv[1];
  if (!px_name) {
    fprintf(stderr, "usage: %s <posix_name>\n", argv[0]);
    return 1;
  }
  // O_TRUNC:若文件存在，则长度被截为0，属性不变
  int oflag = O_RDWR;
  mode_t mode = S_IRUSR | S_IWUSR;
  int fd = shm_open(px_name, oflag, mode);
  if (fd == -1) {
    fprintf(stderr, "shm_open: %s\n", strerror(errno));
    return 1;
  }
  printf("fd: %d \n", fd);
  // Since ftruncate() isn't a standard C function, and you've asked for
  // standards enforcement, you need to define the appropriate feature test
  // macros (see feature_test_macros(7)).
  ftruncate(fd, sizeof(people) * 10);
  p_map = (people *)mmap(NULL, sizeof(people) * 10, PROT_READ | PROT_WRITE,
                         MAP_SHARED, fd, 0);

  if (p_map == (void *)-1) {
    fprintf(stderr, "mmap: %s\n", strerror(errno));
    return 1;
  }
  for (int i = 0; i < 10; i++) {
    printf("name:%s age%d;\n", (*(p_map + i)).name, (*(p_map + i)).age);
  }
  munmap(p_map, sizeof(people) * 10);
  return 0;
}
