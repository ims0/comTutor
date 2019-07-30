#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <errno.h>

#define RETURN_IF_NULL(p) if((p) == NULL){printf ("[%s]:func error!/n", __func__);return;}
#define RETURN_1_IF_NULL(p) if((p) == NULL){printf ("[%s]:func error!/n", __func__);return 1;}

typedef struct
{
    sem_t s1;
    sem_t s2;
    int var;
    time_t end_time;
}PrivInfo;

static void info_init (PrivInfo*, int);
static void info_destroy (PrivInfo* thiz);
static void pthread_func_1 (PrivInfo* thiz);
static void pthread_func_2 (PrivInfo* thiz);

int main (int argc, char** argv)
{
    pthread_t pt_1 = 0;
    pthread_t pt_2 = 0;
    int ret = 0;
    PrivInfo* thiz = NULL;

    thiz = (PrivInfo* )malloc (sizeof (PrivInfo));
    RETURN_1_IF_NULL(thiz)
    info_init(thiz, 300);

    ret = pthread_create(&pt_1, NULL, (void*)pthread_func_1, thiz);
    if (ret != 0)
    {
        perror ("pthread_1_create:");
    }
    else{

    }

    ret = pthread_create(&pt_2, NULL, (void*)pthread_func_2, thiz);
    if (ret != 0)
    {
        perror ("pthread_2_create:");
    }

    pthread_join (pt_1, NULL);
    pthread_join (pt_2, NULL);

    printf ("var: %d\n",thiz->var);
    info_destroy (thiz);

    return 0;
}

static void info_init(PrivInfo* thiz, int times)
{
    RETURN_IF_NULL(thiz );

    thiz->end_time = time(NULL) + times;
    thiz->var = 0;

    sem_init(&thiz->s1, 0, 1);
    sem_init(&thiz->s2, 0, 0);

    return;
}

static void info_destroy (PrivInfo* thiz)
{
    RETURN_IF_NULL (thiz );

    sem_destroy (&thiz->s1);
    sem_destroy (&thiz->s2);

    free (thiz);
    thiz = NULL;
    return;
}

static void pthread_func_1 (PrivInfo* thiz)
{
    RETURN_IF_NULL (thiz );

    while (time(NULL) < thiz->end_time)
    {
        sem_wait (&thiz->s2);
        printf ("pthread1: get sem.\n");
        printf ("pthread1: data process.var:%d\n",thiz->var);
        thiz->var += 1;
        printf ("pthread1: post sem.\n");
        sem_post (&thiz->s1);
        sleep (1);
    }

    return;
}

static void pthread_func_2 (PrivInfo* thiz)
{
    RETURN_IF_NULL (thiz );

    printf("times:%ld\n",time(NULL));
    while (time (NULL) < thiz->end_time)
    {
        sem_wait (&thiz->s1);
        printf ("pthread2: get sem.\n");
        printf ("pthread2: data process.var:%d\n",thiz->var);
        thiz->var += 1;
        printf ("pthread2: post sem.\n");
        sem_post (&thiz->s2);

        sleep (1);
    }

    return;
}
