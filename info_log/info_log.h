/*************************************************************************
    > File Name: info_log.h
    > Author: ims
    > Created Time: 2019年07月19日 星期五 21时04分26秒
 ************************************************************************/

#ifndef INFO_LOG_H
#define INFO_LOG_H

enum ColorIndex{
    red,
    green,
    yellow,
    blue,
    purple,
    deepBlue
};
extern void UnitLog(const char*funcName, int lineNum, int color, const char*format,...);

#define UNIT_LOG(level,format,...)\
    UnitLog(__func__, __LINE__,level,format,##__VA_ARGS__)
#endif
