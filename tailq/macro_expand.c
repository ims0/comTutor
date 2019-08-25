
#include<stdio.h>
#include"queue.h"
#include<stdlib.h>

struct tail_elem
{
    int data;
    struct {
        struct tail_elem *tqe_next;        /* next element */
        struct tail_elem **tqe_prev;    /* address of previous next element */
    }field;
};

//TAILQ_HEAD(tail_head,tail_elem);
struct  tail_head{
    struct tail_elem *tqh_first;        /* first element */
    struct tail_elem **tqh_last;   /* addr of last next element */
};
struct tail_head tHead;
int main()
{

    //TAILQ_INIT(&tHead);
    //
    (&tHead)->tqh_first = NULL;
    (&tHead)->tqh_last = &(&tHead)->tqh_first;
    printf("tHead is empty? %s.\n",TAILQ_EMPTY(&tHead)? "YES":"NO");
    printf("tHead is empty? %s.\n",((&tHead)->tqh_first == NULL)? "YES":"NO");

    struct tail_elem * elm1 =(struct tail_elem *) malloc(sizeof(struct tail_elem));
    elm1->data = 0;
    //TAILQ_INSERT_HEAD(&tHead,elm1,field);
    //{
    if (((elm1)->field.tqe_next = (&tHead)->tqh_first) != NULL)
        (&tHead)->tqh_first->field.tqe_prev = &(elm1)->field.tqe_next;
    else
        (&tHead)->tqh_last = &(elm1)->field.tqe_next;
    (&tHead)->tqh_first = (elm1);
    (elm1)->field.tqe_prev = &(&tHead)->tqh_first;
    //}

    struct tail_elem * elm2 = (struct tail_elem *)malloc(sizeof(struct tail_elem));
    elm2->data = 1;
    //TAILQ_INSERT_AFTER(&tHead,elm1,elm2,field);
    //{
    if (((elm2)->field.tqe_next = (elm1)->field.tqe_next) != NULL)\
        (elm2)->field.tqe_next->field.tqe_prev =         \
            &(elm2)->field.tqe_next;                \
    else                                \
        (&tHead)->tqh_last = &(elm2)->field.tqe_next;        \
    (elm1)->field.tqe_next = (elm2);                \
    (elm2)->field.tqe_prev = &(elm1)->field.tqe_next;    
    //}
    struct tail_elem *var = NULL;
    //TAILQ_FOREACH(var,&tHead,field)
    for ((var) = ((&tHead)->tqh_first); (var); (var) = ((var)->field.tqe_next))
    {
        printf("foreach:%d\n",var->data);
    }

    TAILQ_FOREACH_REVERSE(var,&tHead,tail_head,field)
    {
        printf("foreach_reverse:%d\n",var->data);
    }

    printf("elm1 field=%d\n",TAILQ_NEXT(elm1,field)->data);
    printf("elm1 field=%d\n",((elm1)->field.tqe_next)->data);

    printf("elm2 pre=%d\n",TAILQ_PREV(elm2,tail_head,field)->data);
    printf("elm2 pre=%d\n",(*(((struct tail_head *)((elm2)->field.tqe_prev))->tqh_last))->data);

    printf("tHead is empty? %s.\n",TAILQ_EMPTY(&tHead)? "YES":"NO");

    TAILQ_REMOVE(&tHead,elm1,field);
    if (((elm1)->field.tqe_next) != NULL)                \
        (elm1)->field.tqe_next->field.tqe_prev =         \
            (elm1)->field.tqe_prev;                \
    else                                \
        (&tHead)->tqh_last = (elm1)->field.tqe_prev;        \
    *(elm1)->field.tqe_prev = (elm1)->field.tqe_next;        
    free(elm1);
    TAILQ_REMOVE(&tHead,elm2,field);
    free(elm2);

    printf("tHead is empty? %s.\n",TAILQ_EMPTY(&tHead)? "YES":"NO");

    return 0;
}
