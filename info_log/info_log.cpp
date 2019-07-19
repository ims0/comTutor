/*************************************************************************
    > File Name: info_log.cpp
    > Author: ims
    > Created Time: 2019年07月18日 星期四 23时34分17秒
 ************************************************************************/

#include <stdio.h>
#include <stdarg.h>
#include "info_log.h"

#define MAX_LOG_INFO_LEN (1024)
int levels[] = {
    31,32,33,34,35,36
};
int getColor(int color)
{
    return levels[color];
}
void UnitLog(const char*funcName, int lineNum, int color, const char*format,...)
{
    if( nullptr !=format)
    {
        static char logBuff[MAX_LOG_INFO_LEN+1];
        va_list argList;
        va_start(argList,format);
        vsnprintf(logBuff,MAX_LOG_INFO_LEN,format,argList);
        va_end(argList);
        printf("\e[%d;1m[%s@%d] %s \e[0m \n",getColor(color),funcName,lineNum,logBuff);

    }
}
    
    

