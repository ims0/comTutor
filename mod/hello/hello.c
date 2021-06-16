/*********************************************************
    > File Name: hello.c
    > Author: ims
    > Created Time: Fri Apr  9 19:15:35 2021
 *********************************************************/

#include <linux/init.h>
#include <linux/module.h>
#include <linux/sched.h>

MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("Hcamal");

int hello_init(void)
{
    int size = sizeof(struct task_struct);
    printk(KERN_INFO "Hello World\n");
    printk(KERN_INFO "task_struct size:%d\n", size);
    return 0;
}

void hello_exit(void)
{
    printk(KERN_INFO "Goodbye World\n");
}

module_init(hello_init);
module_exit(hello_exit);
