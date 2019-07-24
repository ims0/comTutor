//
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<sys/types.h>
#include<unistd.h>
void*thfun(void* arg)
{
    pid_t pid= getpid();
    pthread_t tid= pthread_self();
    printf("thread:pid: %u, tid is: %u\n",pid,tid);
    return NULL;
 
}
int main(void)
{
    pid_t pid= getpid();
    pthread_t mtid;
    mtid=pthread_self();
    int err = pthread_create(&mtid,NULL, thfun, NULL);
 
    sleep(1);
    printf("main thread,pid: %u, tid: %u\n", pid,mtid);
    return 0;
}


