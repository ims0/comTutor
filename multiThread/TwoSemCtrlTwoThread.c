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

typedef struct
{
    sem_t sem1;
    sem_t sem2;
    time_t end_time;
}SemStruct;

static void info_init (SemStruct* pSemStru);
static void info_destroy (SemStruct* pSemStru);
static void pthread_func_1 (SemStruct* pSemStru);
static void pthread_func_2 (SemStruct* pSemStru);

int main (int argc, char** argv)
{
    pthread_t pt_1 = 0;
    pthread_t pt_2 = 0;
    SemStruct* pSemStru = (SemStruct* )malloc (sizeof (SemStruct));
    info_init (pSemStru);

    printf("mian pid:%d\n",getpid());
    int ret = pthread_create (&pt_1, NULL, (void*)pthread_func_1, pSemStru);
    return_1_if_fail(ret);
    ret = pthread_create (&pt_2, NULL, (void*)pthread_func_2, pSemStru);
    return_1_if_fail(ret);

    printf("=== pt1:%lu,pt2:%lu ====\n",pt_1, pt_2);
    sleep(1);
    pthread_join (pt_1, NULL);
    pthread_join (pt_2, NULL);

    info_destroy (pSemStru);
    return 0;
}

static void info_init (SemStruct* pSemStru)
{
    return_if_null (pSemStru );
    pSemStru->end_time = time(NULL) + 10;
    sem_init (&pSemStru->sem1, 0, 1);
    sem_init (&pSemStru->sem2, 0, 0);
}

static void info_destroy (SemStruct* pSemStru)
{
    return_if_null (pSemStru );
    sem_destroy (&pSemStru->sem1);
    sem_destroy (&pSemStru->sem2);
    free (pSemStru);
    pSemStru = NULL;
}

static void pthread_func_1 (SemStruct* pSemStru)
{
    return_if_null (pSemStru);

    while (time(NULL) < pSemStru->end_time)
    {
        int temSem = 0;
        sem_getvalue(&pSemStru->sem2, &temSem);
        printf("temSem:%d\n",temSem);
        sem_wait (&pSemStru->sem2);
        pid_t pid = getpid();
        pthread_t tid = pthread_self();
        printf ("pthread1:pid=%d, tid=%lu  get the lock.\n", pid, tid);
        printf ("pthread1: data process.\n");
        sem_post (&pSemStru->sem1);
        //printf ("pthread1: pthread1 unlock\n");

        sleep (1);
    }

    return;
}

static void pthread_func_2 (SemStruct* pSemStru)
{
    return_if_null (pSemStru );

    while (time (NULL) < pSemStru->end_time)
    {
        sem_wait (&pSemStru->sem1);
        pid_t pid = getpid();
        pthread_t tid = pthread_self();
        printf ("pthread2:pid=%d, tid=%lu  get the lock.\n", pid, tid);
        printf ("pthread2: data process.\n");
        sem_post (&pSemStru->sem2);
        sleep (1);
    }
    return;
}
