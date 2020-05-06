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
extern void UnitLog(const char*fileName,const char*funcName, int lineNum, int color, const char*format,...);

#define UNIT_LOG(level,format,...)\
    UnitLog(__FILE__,__func__, __LINE__,level,format,##__VA_ARGS__)
#endif

#define err_log(format,...) UNIT_LOG(rea,format,##__VA_ARGS__)
#define dbg_log(format,...) UNIT_LOG(yellow,format,##__VA_ARGS__)
#define info_log(format,...) UNIT_LOG(green,format,##__VA_ARGS__)
