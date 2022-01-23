/*
   Deprecated create_proc_entry
   Note that the above article uses create_proc_entry which was removed in
   kernel 3.10. Current versions require the following update
diff:
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
#include "const.h"

MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("Liran B.H");

static int g_irq = 20;
module_param(g_irq, int, 0660);

static int g_mode = 1;
module_param(g_mode, int, 0660);

static struct proc_dir_entry *g_proc_dir_entry = NULL;
/*
 * "loff_t" is a "long offset", i.e., a seek position that unifies the crazy diversity of off_t, off64_t, and so o*
 */
static ssize_t proc_write_cb(struct file *file, const char __user *usr_buff, size_t count, loff_t *p_offset) 
{
    int num,length,i,m;
    char buf[BUFF_SIZE];
    if(*p_offset > 0 || count > BUFF_SIZE)
        return -EFAULT;
    if(copy_from_user(buf, usr_buff, count))
        return -EFAULT;
    num = sscanf(buf,"%d %d",&i,&m);
    if(num != 2)
        return -EFAULT;
    g_irq = i; 
    g_mode = m;
    length = strlen(buf);
    *p_offset = length;
    return length;
}

static ssize_t proc_read_cb(struct file *file, char __user *usr_buff, size_t count, loff_t *p_offset) 
{
    char buf[BUFF_SIZE];
    int len=0;
    if(*p_offset > 0 || count < BUFF_SIZE)
        return 0;
    len += sprintf(buf,"g_irq = %d\n",g_irq);
    len += sprintf(buf + len,"g_mode = %d\n",g_mode);

    if(copy_to_user(usr_buff, buf, len))//linux/uaccess.h
        return -EFAULT;
    *p_offset = len;
    return len;
}

static struct file_operations g_file_oper = {
    .owner = THIS_MODULE,
    .read = proc_read_cb,
    .write = proc_write_cb,
};

/*
 * proc_create define in fs/proc/generic.c
 *
 */
static int simple_init(void) {
  g_proc_dir_entry =
      proc_create(PROC_FILE_NAME, S_IRUSR, NULL, &g_file_oper);
  if( NULL == g_proc_dir_entry){
      printk(KERN_ERR "module_init, create %s failed!\n", PROC_FILE);
      return ENOENT;
  }
  printk(KERN_INFO "module_init, create %s succed.\n", PROC_FILE);
  return 0;
}

static void simple_exit(void) {
  proc_remove(g_proc_dir_entry);
  printk(KERN_WARNING "module_exit, remove %s\n", PROC_FILE);
}

module_init(simple_init);
module_exit(simple_exit);
