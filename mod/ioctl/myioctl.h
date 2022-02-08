/*********************************************************
  > File Name: myioctl.h
  > Author: ims
  > Created Time: Tue May 25 23:45:46 2021
 *********************************************************/
#ifndef MYIOCTL_H
#define MYIOCTL_H
#include <linux/ioctl.h>

//#define FELLOW_MISC_MAJOR 199
//#define FELLOW_MISC_NR 2
struct miscdata {
    int val;
    char *str;
    unsigned int size;
};

#define FELLOW_MISC_IOC_MAGIC 'f'
#define FELLOW_MISC_IOC_PRINT _IO(FELLOW_MISC_IOC_MAGIC, 1)
#define FELLOW_MISC_IOC_GET _IOR(FELLOW_MISC_IOC_MAGIC, 2, struct miscdata)
#define FELLOW_MISC_IOC_SET _IOW(FELLOW_MISC_IOC_MAGIC, 3, struct miscdata)
#define FELLOW_MISC_IOC_MAXNR 3

const char *const misc_dev_name = "misc_dev_one";
//原文链接：https://blog.csdn.net/wjlinux21/article/details/111691691
#endif
