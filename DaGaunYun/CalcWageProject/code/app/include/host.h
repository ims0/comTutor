/*************************************************************************
    > File Name: host.h
    > Author: ims
    > Created Time: 2019年07月15日 星期一 23时53分23秒
 ************************************************************************/
#include"servant.h"
#include<memory>
#ifndef HOST_H
#define HOST_H

class Host{
public:
    Host()
    {

    }
    virtual int hostPayWage(std::shared_ptr<Servant>&,int quarter, int month)=0;
    virtual ~Host(){}
};

class BaoYu:public Host{
public:
    virtual int hostPayWage(std::shared_ptr<Servant>&,int quarter, int month);
    virtual ~BaoYu(){}
};

class JiaMu:public Host{
public:
    virtual int hostPayWage(std::shared_ptr<Servant>&,int quarter, int month);
    virtual ~JiaMu(){}
};
class JiaZheng:public Host{
public:
    virtual int hostPayWage(std::shared_ptr<Servant>&,int quarter, int month);
    virtual ~JiaZheng(){}
};

#endif
