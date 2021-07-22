/*********************************************************
    > File Name: main.c
    > Author: ims
    > Created Time: Wed 21 Jul 2021 11:22:15 PM CST
 *********************************************************/

// test.c
#include <stdio.h>
#include <stdlib.h>

extern int getcnt();
int main()
{
    printf("malloc cnt start:%d\n",getcnt());
    int *ptr = (int *)malloc(sizeof(int));
    if (NULL == ptr) {
        printf("malloc fails\n");
    }
    else {
        ptr[0] =1;
        printf("malloc ok \n");
    }
    free(ptr);
    printf("malloc cnt end:%d\n",getcnt());
    return 0;
}
