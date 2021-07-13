/*
https://juejin.cn/post/6867303928120229896#heading-15
   Deprecated create_proc_entry

   Note that the above article uses create_proc_entry which was removed in
   kernel 3.10. Current versions require the following update

   -    entry = create_proc_entry("sequence", 0, NULL);
   -    if (entry)
   -        entry->proc_fops = &ct_file_ops;
   +    entry = proc_create("sequence", 0, NULL, &ct_file_ops);
   */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/spinlock.h>
#include <linux/mutex.h>

//DEFINE_MUTEX(mutex_lock)
struct mutex mutexname ;
    
//mutex_init(&mutexname);

static int major = 250;
static int minor = 0;
static dev_t devno;
static struct class *cls;
static struct device *test_device;
static spinlock_t lock;
static int flage = 1;
#define DEAD 1
static int hello_open (struct inode *inode, struct file *filep)
{
    spin_lock(&lock);
    if(flage !=1)
    {
         spin_unlock(&lock);
         return -EBUSY;
    }
    flage =0;
    #if DEAD
    #elif
    spin_unlock(&lock);
    #endif
    return 0;
}
static int hello_release (struct inode *inode, struct file *filep)
{
    flage = 1;
    #if DEAD
    spin_unlock(&lock);
    #endif
    return 0;
}
static struct file_operations hello_ops =
{
    .open = hello_open,
    .release = hello_release,
};
static int hello_init(void)
{
    int result;
    printk("hello_init \n");
    result = register_chrdev( major, "hello", &hello_ops);
    if(result < 0)
    {
        printk("register_chrdev fail \n");
        return result;
    }
    devno = MKDEV(major,minor);
    cls = class_create(THIS_MODULE,"helloclass");
    if(IS_ERR(cls))
    {
        unregister_chrdev(major,"hello");
        return result;
    }
    test_device = device_create(cls,NULL,devno,NULL,"test");
    if(IS_ERR(test_device ))
    {
        class_destroy(cls);
        unregister_chrdev(major,"hello");
        return result;
    }
    spin_lock_init(&lock);
    return 0;
}
static void hello_exit(void)
{
    printk("hello_exit \n");
    device_destroy(cls,devno);
    class_destroy(cls);
    unregister_chrdev(major,"hello");
    return;
}
module_init(hello_init);
module_exit(hello_exit);
MODULE_LICENSE("GPL");

