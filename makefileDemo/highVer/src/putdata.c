/*************************************************************************
    > File Name: putdata.c
    > Author: ims
    > Created Time: 2018年10月28日 星期日 21时47分31秒
 ************************************************************************/

#include<stdio.h>
#include"putdata.h"

void putdata(int n, int k, double data)
{
	char prompt[100];
	sprintf(prompt,"From %d fetch %d have %.0lf kinds ways\n",n, k,data);
	puts(prompt);
}
