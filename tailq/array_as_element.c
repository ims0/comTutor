#include<stdio.h>
#include<stdlib.h>
#include<sys/queue.h>

#define STRU_LEN (100)

struct TailElem
{
    int data;
    TAILQ_ENTRY(TailElem) field;
};
TAILQ_HEAD(TailHead,TailElem);

typedef struct TailElem TailElem;
typedef struct TailHead TailHead;


typedef struct{
    int usedNum;
    TailElem tailElem[STRU_LEN];
    TailHead usedHead;
    TailHead freeHead;
}DataObj;
int main()
{
    DataObj dataObj;
    TAILQ_INIT(&dataObj.freeHead);
    TAILQ_INIT(&dataObj.usedHead);
    //add elem to free
    for( int loop = 0 ; loop<STRU_LEN ; ++loop )
    {
        TAILQ_INSERT_TAIL(&dataObj.freeHead, &dataObj.tailElem[loop], field);
    }
    //use elem
    dataObj.usedNum = 0;
    for( int loop = 0 ; loop<10 ; ++loop )
    {
        TailElem *temElem = TAILQ_FIRST(&dataObj.freeHead);
        TAILQ_REMOVE(&dataObj.freeHead, temElem, field);

        TAILQ_INSERT_TAIL(&dataObj.usedHead, temElem, field);
        temElem->data = dataObj.usedNum++;
    }
    TailElem *loopElem;
    TAILQ_FOREACH(loopElem, &dataObj.usedHead, field)
    {
        printf("elem:%d\n", loopElem->data);
    }
    return 0;
}
