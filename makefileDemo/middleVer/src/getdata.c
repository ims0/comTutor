/*************************************************************************
    > File Name: getdata.c
    > Author: ims
    > Created Time: 2018年10月28日 星期日 21时33分24秒
 ************************************************************************/

#include<stdio.h>
#include"getdata.h"

int input(char *prompt)
{
	int x;
	do{

		puts(prompt);
		scanf("%d",&x);
	}
	while(x<=0||x>FACMAX);
	return x;
}
void getdata(int *n, int *k)
{
	
	char prompt[100];
	sprintf(prompt,"input num of total sample(<%d)",FACMAX);
	*n = input(prompt);
	do{
		sprintf(prompt,"input num of get sample (<%d <%d)",FACMAX,*n);
		*k=input(prompt);
	}
	while(*n<*k);
}

