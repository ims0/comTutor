/*********************************************************
    > File Name: mutex.c
    > Author: ims
    > Created Time: Tue 13 Jul 2021 11:17:21 AM CST

src:https://www.geeksforgeeks.org/mutex-lock-for-linux-thread-synchronization/
    pthread_mutex_lock online doc
    https://code.woboq.org/userspace/glibc/nptl/pthread_mutex_lock.c.html

typedef union
{
  struct __pthread_mutex_s __data;
  char __size[__SIZEOF_PTHREAD_MUTEX_T];
  long int __align;
} pthread_mutex_t;
 *********************************************************/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

pthread_t tid[2];
int counter;
pthread_mutex_t lock;
/* Define ALIASNAME as a weak alias for NAME.
   If weak aliases are not available, this defines a strong alias.  */
# define weak_alias(name, aliasname) _weak_alias (name, aliasname)
# define _weak_alias(name, aliasname) \
  extern __typeof (name) aliasname __attribute__ ((weak, alias (#name)));
#ifdef FAKE
int pthread_mutex_lock (pthread_mutex_t *__mutex)
{
    puts("fake lock");
    return 0;
}

int pthread_mutex_unlock (pthread_mutex_t *__mutex)
{
    puts("fake unlock");
    return 0;
}
#endif

void __foo(){ printf("from:%s\n", __func__);}
//And weak alias
//int foo()__attribute__((weak, alias("_foo")));
weak_alias (__foo, foo)


void* trythis(void* arg)
{
    (void)arg;
    pthread_mutex_lock(&lock);

    counter += 1;
    printf("\n Job %d has started\n", counter);

    sleep(1);

    printf("\n Job %d has finished\n", counter);

    pthread_mutex_unlock(&lock);

    return NULL;
}

int main(void)
{
    int i = 0;
    int error;

    if (pthread_mutex_init(&lock, NULL) != 0) {
        printf("\n mutex init has failed\n");
        return 1;
    }

    while (i < 2) {
        error = pthread_create(&(tid[i]),
                               NULL,
                               &trythis, NULL);
        if (error != 0)
            printf("\nThread can't be created :[%s]",
                   strerror(error));
        i++;
    }

    pthread_join(tid[0], NULL);
    pthread_join(tid[1], NULL);
    pthread_mutex_destroy(&lock);

    foo();
    return 0;
}
