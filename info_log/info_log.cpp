/*************************************************************************
    > File Name: info_log.cpp
    > Author: ims
    > Created Time: 2019年07月18日 星期四 23时34分17秒
 ************************************************************************/

#include <stdio.h>
#include <memory.h>
#include <stdarg.h>

#define MAX_LOG_INFO_LEN (1024)
int levels[] = {
    31,32,33,34,35,36
};
enum ColorIndex{
    red,
    green,
    yellow,
    blue,
    purple,
    deepBlue
};
int getColor(int color)
{
    return levels[color];
}
void unitLog(const char*funcName, int lineNum, int color, const char*format,...)
{

    if( nullptr ==format)
    {
        static char logBuff[MAX_LOG_INFO_LEN+1];
        va_list argList;
        va_start(argList,format);
        vsnprintf(logBuff,MAX_LOG_INFO_LEN,format,argList);
        va_end(argList);
        printf("\e[%d,1m[%si@%d]%s\n\e[0m",getColor(color),funcName,lineNum,logBuff);

    }
}
#define UNI_LOG(level,format,...)\
    unitLog(__func__, __LINE__,level,format,##__VA_ARGS__)
    
int main()
{
    
    UNI_LOG(red, "hello",1);

}
    

