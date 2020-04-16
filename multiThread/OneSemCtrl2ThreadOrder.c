#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/syscall.h>
/*     
 *int sem_init(sem_t *sem, int pshared, unsigned int value);
 *sem ：指向信号量对象
 *pshared : 指明信号量的类型。不为0时此信号量在进程间共享，否则只能为当前进程的所有线程共享。
 *value : 指定信号量值的大小
 *pthread_t unsigned long
 *sem_t  long
 */
#define return_if_null(p) if((p) == 0){printf ("[%s]: null ptr!/n", __func__);return;}
#define return_1_if_fail(p) if((p) != 0){perror ("fail!/n");return 1;}

static void pthread_func_write (char mem[]);
static void pthread_func_read (char mem[]);

sem_t sem ;
int main (int argc, char** argv)
{
    sem_init (&sem, 1, 0);
    pthread_t pt_1 = 0;
    pthread_t pt_2 = 0;
    char shareStackMem[100];
    printf("mian pid:%d\n",getpid());
    int ret = pthread_create (&pt_1, NULL, (void*)pthread_func_write, shareStackMem);
    return_1_if_fail(ret);
    ret = pthread_create (&pt_2, NULL, (void*)pthread_func_read, shareStackMem);
    return_1_if_fail(ret);

    printf("=== pt1:%lu,pt2:%lu ====\n",pt_1, pt_2);
    sleep(1);
    pthread_join (pt_1, NULL);
    pthread_join (pt_2, NULL);

    sem_destroy (&sem);
    return 0;
}

static void pthread_func_write (char shareMem[])
{
    sleep(1);
    return_if_null (shareMem);
    int temSem = 0;
    sem_getvalue(&sem, &temSem);
    printf("[write] temSem:%d\n",temSem);
    pid_t pid = getpid();
    pthread_t tid = pthread_self();
    printf ("pthread1:pid=%d, tid=%lu write data,wait 2s.\n", pid, tid);
    snprintf(shareMem,100,"%s","this wirte date.");
    sleep(2);
    sem_post(&sem);
    return;
}

static void pthread_func_read (char shareMem[])
{
    return_if_null (shareMem);
    sem_wait (&sem);
    pid_t pid = getpid();
    pthread_t tid = pthread_self();
    printf ("pthread2:pid=%d, tid=%lu  get the lock.\n", pid, tid);
    printf ("pthread2: read shareMem:%s.\n", shareMem);
    return;
}
