/*************************************************************************
    > File Name: main.c
    > Author: ims
    > Created Time: 2018年10月28日 星期日 21时29分24秒
 ************************************************************************/

#include<stdio.h>
#include "define.h"
#include "calc.h"
#include "getdata.h"
#include "putdata.h"



int main()
{
	
	int n,k;
	double c;
	getdata(&n, &k);
	c = calc(n,k);
	putdata(n,k,c);
	return 0;
}
