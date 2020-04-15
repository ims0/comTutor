

[toc]



## 信号量与互斥锁的区别
作用域

    信号量: 进程间或线程间
    互斥锁: 线程间
作用：

    信号量：线程同步
    互斥锁：线程互斥
数值:

    信号量：可以为非负整数
    互斥量: 只能为 0/1
创建与释放的：

    互斥量的加锁和解锁必须由同一线程分别对应使用，信号量可以由一个线程释放，另一个线程得到


## 不同操作系统中提供的Mutex函数:
动作\系统 | Win32 | Linux | Solaris
------|-------|-------|--------
创建 | CreateMutex | pthread_mutex_init | mutex_init
加锁 | WaitForSingleObject | pthread_mutex_lock | mutex_lock
解锁 | ReleaseMutex | pthread_mutex_unlock | mutex_unlock
销毁 | CloseHandle | pthread_mutex_destroy | mutex_destroy

## 信号量

动作\系统    Win32  POSIX 
创建    CreateSemaphore     sem_init 
等待    WaitForSingleObject     sem _wait 
释放    ReleaseMutex    sem_post 
试图等待    WaitForSingleObject     sem_trywait 
销毁    CloseHandle    sem_destroy


### 实例，信号量控制资源被线程访问的顺序

