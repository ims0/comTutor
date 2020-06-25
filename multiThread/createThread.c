#include<stdio.h>
#include<memory.h>
#include<pthread.h>
#include<sys/types.h>
#include<unistd.h>

typedef struct 
{
    int data[100];
    int len;
}ThArgs;

void* thFun(void* arg)
{
    pid_t pid= getpid();
    pthread_t tid= pthread_self();
    ThArgs *thArgs = (ThArgs*)arg;
    thArgs->len = 10;
    printf("thread:pid: %u, tid is: %lu\n",pid,tid);
    pthread_detach(pthread_self());
    return NULL;
}

int main(void)
{
    ThArgs thArgs;
    memset(&thArgs,0,sizeof(thArgs));

    int res = pthread_create(&mainTid, NULL, thFun, &thArgs);
 
    sleep(1);

    pid_t mainPid= getpid();
    pthread_t mainTid=pthread_self();
    printf("local var as args now value is: %d\n", thArgs.len);
    printf("main thread,pid: %u, tid: %lu\n", mainPid, mainTid);
    return res; 
}


