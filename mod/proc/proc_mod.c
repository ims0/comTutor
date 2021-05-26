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
#define BUFSIZE  100


MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("Liran B.H");

static int irq=20;
module_param(irq,int,0660);

static int mode=1;
module_param(mode,int,0660);

const char *file_name = "mydev";
static struct proc_dir_entry *entry;

static ssize_t mywrite(struct file *file, const char __user *ubuf, size_t count, loff_t *ppos) 
{
    int num,c,i,m;
    char buf[BUFSIZE];
    if(*ppos > 0 || count > BUFSIZE)
        return -EFAULT;
    if(copy_from_user(buf, ubuf, count))
        return -EFAULT;
    num = sscanf(buf,"%d %d",&i,&m);
    if(num != 2)
        return -EFAULT;
    irq = i; 
    mode = m;
    c = strlen(buf);
    *ppos = c;
    return c;
}

static ssize_t myread(struct file *file, char __user *ubuf,size_t count, loff_t *ppos) 
{
    char buf[BUFSIZE];
    int len=0;
    if(*ppos > 0 || count < BUFSIZE)
        return 0;
    len += sprintf(buf,"irq = %d\n",irq);
    len += sprintf(buf + len,"mode = %d\n",mode);

    if(copy_to_user(ubuf,buf,len))//linux/uaccess.h
        return -EFAULT;
    *ppos = len;
    return len;
}
//include/linux/fs.h
static struct file_operations myops = 
{
    .owner = THIS_MODULE,
    .read = myread,
    .write = mywrite,
};


static int simple_init(void)
{
    entry=proc_create(file_name, 0660, NULL, &myops);
    printk(KERN_ALERT "hello, create /proc/%s\n", file_name);
    return 0;
}

static void simple_cleanup(void)
{
    proc_remove(entry);
    printk(KERN_WARNING "bye, remove /proc/%s\n", file_name);
}

module_init(simple_init);
module_exit(simple_cleanup);

