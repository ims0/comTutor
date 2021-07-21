
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

int main(int argc , char *args[]){
        //定义互斥锁
        pthread_mutex_t mutex; 
        if(argc < 2){
                printf("input mutex type\n");
                return -1;
        }
        //定义互斥锁属性
        pthread_mutexattr_t mutexattr;
        //初始化互斥锁属性
        pthread_mutexattr_init(&mutexattr);
        //设置互斥锁类型，error:检错互斥锁，normal:标准互斥锁，recursive:递归互斥锁
        if(!strcmp(args[1] , "error")){
                pthread_mutexattr_settype(&mutexattr , PTHREAD_MUTEX_ERRORCHECK);
                printf("set error succes\n");
        }else if(!strcmp(args[1], "normal")){
                pthread_mutexattr_settype(&mutexattr , PTHREAD_MUTEX_NORMAL);
                printf("set normal succes\n");
        }else if(!strcmp(args[1], "recursive")){
                pthread_mutexattr_settype(&mutexattr , PTHREAD_MUTEX_RECURSIVE);
                printf("set recursive\n");
        }
        //初始化互斥锁
        pthread_mutex_init(&mutex , &mutexattr);
        //第一次上锁
        if(pthread_mutex_lock(&mutex) != 0){
                printf("lock failed\n");
        }else{
                printf("lock succes\n");
        }
        //第二次上锁
        if(pthread_mutex_lock(&mutex) != 0){
                printf("lock failed\n");
        }else{
                printf("lock succes\n");
        }
        //加锁几次，同样也要释放几次
        pthread_mutex_unlock(&mutex);
        pthread_mutex_unlock(&mutex);
        //销毁互斥锁属性和互斥锁
        pthread_mutexattr_destroy(&mutexattr);
        pthread_mutex_destroy(&mutex);
        return 0;
}

