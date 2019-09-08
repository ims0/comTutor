/*************************************************************************
  > File Name: print.c
  > Author: ims
  > Created Time: 2019年09月08日 星期日 19时19分43秒
 ************************************************************************/

#include<stdio.h>
#include <unistd.h>
void print()
{
    static int cnt=0;
    switch (cnt++)
    {
        case 0:printf("\b\\");break; 
        case 1:printf("\b|");break; 
        case 2:printf("\b/");break; 
        case 3:printf("\b—");break; 
    }
    fflush(stdout);
    if(cnt == 4)
        cnt=0;
}
int main()
{
    while(1)
    {
        print();
        sleep(1);
    }
    return 0;
}
