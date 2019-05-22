/*************************************************************************
    > File Name: calc.c
    > Author: ims
    > Created Time: 2018年10月28日 星期日 21时43分12秒
 ************************************************************************/

#include"calc.h"

double factorial(int n)
{
	double s=1;
	int i;
	for(i=1; i<=n; i++)
	{
		s = s*i;
	}
	return s;
}
double calc(int n,int k)
{
	return factorial(n)/(factorial(k)*factorial(n-k));
}
