/*************************************************************************
  > File Name: demo.c
  > Author: ims
  > Created Time: 2019年08月08日 星期四 07时05分17秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include"sys/queue.h"

struct tail_elem 
{
    TAILQ_ENTRY(tail_elem) entry;
    int data;
};

TAILQ_HEAD(tail_head,tail_elem);

struct tail_head list1;

int main()
{

    TAILQ_INIT(&list1);
    printf("list1 is empty? %s.\n",TAILQ_EMPTY(&list1)? "YES":"NO");

    struct tail_elem * elm1 =(struct tail_elem *) malloc(sizeof(struct tail_elem));
    elm1->data = 0;
    TAILQ_INSERT_HEAD(&list1,elm1,entry);

    struct tail_elem * elm2 = (struct tail_elem *)malloc(sizeof(struct tail_elem));
    elm2->data = 1;
    TAILQ_INSERT_AFTER(&list1,elm1,elm2,entry);

    struct tail_elem *var = NULL;
    TAILQ_FOREACH(var,&list1,entry)
    {
        printf("foreach:%d\n",var->data);
    }

    TAILQ_FOREACH_REVERSE(var,&list1,tail_head,entry)
    {
        printf("foreach_reverse:%d\n",var->data);
    }

    printf("elm1 entry=%d\n",TAILQ_NEXT(elm1,entry)->data);

    printf("elm2 pre=%d\n",TAILQ_PREV(elm2,tail_head,entry)->data);

    printf("list1 is empty? %s.\n",TAILQ_EMPTY(&list1)? "YES":"NO");

    TAILQ_REMOVE(&list1,elm1,entry);
    free(elm1);
    TAILQ_REMOVE(&list1,elm2,entry);
    free(elm2);

    printf("list1 is empty? %s.\n",TAILQ_EMPTY(&list1)? "YES":"NO");

    return 0;
}
