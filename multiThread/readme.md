

[toc]



## 信号量与互斥锁的区别
作用域

    信号量: 进程间或线程间
    互斥锁: 线程间
作用：

    信号量：线程同步
    互斥锁：线程互斥
 + 信号量：是多线程同步用的，一个线程完成了某一动作就通过信号告诉别的线程，别的线程在进行某些动作。是Unix进程间通信的方式之一。

 + 互斥量：是多线程互斥用的，比如说，一个线程占用了某一资源，那么别的线程就无法访问，直到这个线程离开，其他线程才开始可以利用这个资源。是同一进程下的多线程的五种同步方式之一。

数值:

    信号量：可以为非负整数
    互斥量: 只能为 0/1
创建与释放的：

    互斥量的加锁和解锁必须由同一线程分别对应使用，信号量可以由一个线程释放，另一个线程得到
### 其它对区别的解释
 ①.互斥量用于线程的互斥，信号量用于线程的同步，这是根本区别。也就是互斥和同步的区别。

互斥：是指某一资源同时只允许一个访问者对其进行访问，具有唯一性和排它性。但互斥无法限制访问者对资源的访问顺序，即访问是无序的。
同步：是指在互斥的基础上（大多数情况），通过其它机制实现访问者对资源的有序访问。在大多数情况下，同步已经实现了互斥，特别是所有写入资源的情况必定是互斥的。少数情况是指可以允许多个访问者同时访问资源

  ②.互斥量值只能为0/1，信号量值可以为非负整数。
即一个互斥量只能用于一个资源的互斥访问，它不能实现多个资源的多线程互斥问题。信号量可以实现多个同类资源的多线程互斥和同步。当信号量为单值信号量是，也可以完成一个资源的互斥访问。 因此，我们也可以把互斥量看成是信号量的一种特殊方式。

  ③. 互斥量的加锁和解锁必须由同一线程分别对应使用，信号量可以由一个线程释放，另一个线程得到。

## 不同操作系统中提供的Mutex函数:
动作\系统 | Win32 | Linux | Solaris
------|-------|-------|--------
创建 | CreateMutex | pthread_mutex_init | mutex_init
加锁 | WaitForSingleObject | pthread_mutex_lock | mutex_lock
解锁 | ReleaseMutex | pthread_mutex_unlock | mutex_unlock
销毁 | CloseHandle | pthread_mutex_destroy | mutex_destroy

## 信号量

动作\系统 | Win32 | POSIX 
------|-------|-------
创建 | CreateSemaphore | sem_init 
等待 | WaitForSingleObject | sem _wait 
释放 | ReleaseMutex | sem_post 
试图等待 | WaitForSingleObject | sem_trywait 
销毁 | CloseHandle | sem_destroy


### 实例，信号量控制资源被线程访问的顺序

```
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


```

