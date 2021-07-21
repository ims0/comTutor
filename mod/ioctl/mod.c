/*
   Deprecated create_proc_entry

   Note that the above article uses create_proc_entry which was removed in
   kernel 3.10. Current versions require the following update

   -    entry = create_proc_entry("sequence", 0, NULL);
   -    if (entry)
   -        entry->proc_fops = &ct_file_ops;
   +    entry = proc_create("sequence", 0, NULL, &ct_file_ops);
   */
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>
#include <linux/kernel.h>   
#include <linux/proc_fs.h>
#include <asm/uaccess.h>
#include <linux/uaccess.h>

#include <linux/types.h>
#include <linux/miscdevice.h>
#include <linux/platform_device.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/mm.h>
#include <linux/slab.h>
#include <asm/io.h>
#include <linux/ioctl.h>
#include "myioctl.h"


MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("Liran");
static DEFINE_SEMAPHORE(s_sem);

struct fellowmisc_dev{
    struct miscdevice misc;
    struct miscdata data;
};

struct fellowmisc_dev *fellowmisc_devp;

int fellowmisc_open(struct inode *inode, struct file *filep)
{
    filep->private_data = fellowmisc_devp;
    return down_interruptible(&s_sem);
}


int fellowmisc_release(struct inode *inode, struct file *filep)
{
    up(&s_sem);
    return 0;
}

long fellowmisc_ioctl(struct file *filep,unsigned int cmd,unsigned long arg)
{
    int ret = 0;
    struct fellowmisc_dev *devp = (struct fellowmisc_dev *)(filep->private_data);
    if (_IOC_TYPE(cmd) != FELLOW_MISC_IOC_MAGIC)
        return -EINVAL;
    if (_IOC_NR(cmd) > FELLOW_MISC_IOC_MAXNR)
        return -EINVAL;

    switch(cmd)
    {
        case FELLOW_MISC_IOC_PRINT:
            printk("FELLOW_MISC_IOC_PRINT\n");
            printk("val:%d, size: %d, str: %s\n", devp->data.val, devp->data.size, devp->data.str);
            break;
        case FELLOW_MISC_IOC_SET:
            printk("FELLOW_MISC_IOC_SET\n");
            ret = copy_from_user((unsigned char*)&(devp->data), (unsigned char *)arg, sizeof(struct miscdata));
            printk("set val:%d, size: %d, str: %s\n", devp->data.val, devp->data.size, devp->data.str);
            break;

        case FELLOW_MISC_IOC_GET:
            printk("FELLOW_MISC_IOC_GET\n");
            ret = copy_to_user((unsigned char*)arg,(unsigned char*)&(devp->data), sizeof(struct miscdata));
            break;
        default:
            return -EINVAL;

    }
    return ret;
}

static const struct file_operations fellowmisc_fops ={
    .owner = THIS_MODULE,
    .open = fellowmisc_open,
    .release = fellowmisc_release,
    .unlocked_ioctl = fellowmisc_ioctl,
};
static struct miscdevice fellow_misc = {
    .minor = MISC_DYNAMIC_MINOR,
    .name = "fellowplat",
    .fops = &fellowmisc_fops,
};

static struct platform_device *fellow_platform_device;
static int fellow_plat_drv_probe(struct platform_device *dev)
{
    int error;
    printk("fellow_plat_drv_probe\n");
    fellowmisc_devp = kmalloc(sizeof(struct fellowmisc_dev), GFP_KERNEL);
    if (!fellowmisc_devp)
    {
        error = -ENOMEM;
        return error;
    }

    memset(&(fellowmisc_devp->data), 0, sizeof(fellowmisc_devp->data));
    fellowmisc_devp->misc = fellow_misc;
    error = misc_register(&fellow_misc);
    printk("fellow_plat_drv_probe ret:%d\n", error);
    return error;
}

static int fellow_plat_drv_remove(struct platform_device *dev)
{
    //int error;

    if(fellowmisc_devp)
    {
        kfree(fellowmisc_devp);
    }
    //error = misc_deregister(&fellow_misc);
    misc_deregister(&fellow_misc);
    return 0;
}

static struct platform_driver fellow_platform_driver = {
    .driver = {
        .name = "fellow",
    },
    .probe = fellow_plat_drv_probe,
    .remove = fellow_plat_drv_remove,
};

static int fellowplat_init(void)
{
    int error;
    printk("fellowplat_init\n");
    printk("fellow register driver\n");
    error = platform_driver_register(&fellow_platform_driver);//注册platform driver

    if (error)
        return error;

    fellow_platform_device = platform_device_alloc("fellow", -1);//名字与platform driver相同。

    if (!fellow_platform_device) {
        error = -ENOMEM;
        goto err_driver_unregister;
    }

    printk("fellow register device\n");

    error = platform_device_add(fellow_platform_device);//添加platform device

    if (error)
        goto err_free_device;


    return 0;

err_free_device:
    platform_device_put(fellow_platform_device);
err_driver_unregister:
    platform_driver_unregister(&fellow_platform_driver);
    return error;
}

static void fellowplat_exit(void)
{
    platform_device_unregister(fellow_platform_device);
    platform_driver_unregister(&fellow_platform_driver);
}


module_init(fellowplat_init);
module_exit(fellowplat_exit);

