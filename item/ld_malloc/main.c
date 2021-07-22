/*********************************************************
    > File Name: main.c
    > Author: ims
    > Created Time: Wed 21 Jul 2021 11:22:15 PM CST
 *********************************************************/

// test.c
#include <stdio.h>
#include <stdlib.h>

int main()
{
    int *ptr = (int *)malloc(sizeof(int));
    if (NULL == ptr) {
        printf("malloc fails\n");
    }
    else {
        ptr[0] =1;
        printf("malloc ok var:%d\n", ptr[0]);
    }
    free(ptr);
    return 0;
}
