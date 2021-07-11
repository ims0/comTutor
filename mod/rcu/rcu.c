/*
 * 原文链接：https://blog.csdn.net/wendowswd/article/details/90575606
 * https://www.kernel.org/doc/Documentation/RCU/whatisRCU.txt
 * https://www.cnblogs.com/sky-heaven/p/11082428.html  -->kthread_run
*/
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/spinlock.h>
#include <linux/mm.h>
#include <linux/slab.h>
#include <linux/rcupdate.h>

#include <linux/delay.h>
#include <linux/sched.h>   //wake_up_process()
#include <linux/sched/task.h>   //wake_up_process()
#include <linux/kthread.h> //kthread_create()、kthread_run()
#include <linux/err.h> //IS_ERR()、PTR_ERR()
static struct task_struct * MyRunThread = NULL;
static struct task_struct * threadByCreate = NULL;
struct foo {
    int a;
    char b;
    long c;
    struct rcu_head rcu;
};

DEFINE_SPINLOCK(foo_mutex);

struct foo g_foo = {0,0,0};
struct foo *gbl_foo = &g_foo;
    /*
     * Create a new struct foo that is the same as the one currently
     * pointed to by gbl_foo, except that field "a" is replaced
     * with "new_a".  Points gbl_foo to the new structure, and
     * frees up the old structure after a grace period.
     *
     * Uses rcu_assign_pointer() to ensure that concurrent readers
     * see the initialized version of the new structure.
     *
     * Uses synchronize_rcu() to ensure that any readers that might
     * have references to the old structure complete before freeing
     * the old structure.
     */
void foo_update_a(int new_a)
{
    struct foo *new_fp;
    struct foo *old_fp;
    new_fp = kmalloc(sizeof(*new_fp), GFP_KERNEL);
    spin_lock(&foo_mutex);
    old_fp = gbl_foo;
    *new_fp = *old_fp;
    new_fp->a = new_a;
    rcu_assign_pointer(gbl_foo, new_fp);
    // gbl_foo = new_fp;
    spin_unlock(&foo_mutex);
    synchronize_rcu();
    kfree(old_fp);
}

int foo_get_a(void)
{
    int retval;
    struct foo *fp;
    /*没有使用加锁，原子变量，或者全局变量，而是简单地关抢占。*/
    rcu_read_lock();
    //fp = gbl_foo;
    fp = rcu_dereference(gbl_foo);
    retval = fp->a;
    rcu_read_unlock();
    return retval;
 }
    /*
     * Create a new struct foo that is the same as the one currently
     * pointed to by gbl_foo, except that field "a" is replaced
     * with "new_a".  Points gbl_foo to the new structure, and
     * frees up the old structure after a grace period.
     *
     * Uses rcu_assign_pointer() to ensure that concurrent readers
     * see the initialized version of the new structure.
     *
     * Uses call_rcu() to ensure that any readers that might have
     * references to the old structure complete before freeing the
     * old structure.
     */
//  The foo_reclaim() function might appear as follows:

void foo_reclaim(struct rcu_head *rp)
{
    struct foo *fp = container_of(rp, struct foo, rcu);
    //foo_cleanup(fp->a); if a is ptr 
    kfree(fp);
}
void foo_update_a_no_blk(int new_a)
{
    struct foo *new_fp;
    struct foo *old_fp;

    new_fp = kmalloc(sizeof(*new_fp), GFP_KERNEL);
    spin_lock(&foo_mutex);
    old_fp = rcu_dereference_protected(gbl_foo, lockdep_is_held(&foo_mutex));
    *new_fp = *old_fp;
    new_fp->a = new_a;
    rcu_assign_pointer(gbl_foo, new_fp);
    spin_unlock(&foo_mutex);
    call_rcu(&old_fp->rcu, foo_reclaim);
}
    /*
    * If the callback for call_rcu() is not doing anything more than calling
    * kfree() on the structure, you can use kfree_rcu() instead of call_rcu()
    * to avoid having to write your own callback::

    *     kfree_rcu(old_fp, rcu);
    */
void foo_update_a_no_blk_kfree(int new_a)
{
    struct foo *new_fp;
    struct foo *old_fp;

    new_fp = kmalloc(sizeof(*new_fp), GFP_KERNEL);
    spin_lock(&foo_mutex);
    old_fp = rcu_dereference_protected(gbl_foo, lockdep_is_held(&foo_mutex));
    *new_fp = *old_fp;
    new_fp->a = new_a;
    rcu_assign_pointer(gbl_foo, new_fp);
    spin_unlock(&foo_mutex);
    kfree_rcu(old_fp, rcu); // only call kfree in call_rcu
}

static int test_func (void *name){

    int i = 0 ;
    printk("thread func:%s entry, arg:%s\n", __func__,(char*)name);
    while(!kthread_should_stop())
    {
        ssleep(5);
        printk(" %s loop:%d\n", __func__, i++);
    }
    printk(" %s exit\n", __func__);
    return 0;
}

static int  create_thread (void) {

    int rc = 0;
    threadByCreate = kthread_create(test_func, "kthread_create_date", "rcu_th_create");
    if (IS_ERR(threadByCreate))
    {
        rc = PTR_ERR(threadByCreate);
        printk(" error %d create thread_name thread", rc);
        return 1;
    }
    kthread_bind(threadByCreate,1);                             //这里是绑定CPU1上哦，
    wake_up_process(threadByCreate);                            //这里需要wakeup ，线程才开始执行

    return 0;
}


static int  create_thread_simple (void) {
      int rc = 0;

      MyRunThread = kthread_run(test_func, "kthread_run_date","rcu_th_run");
      if (IS_ERR(MyRunThread))
      {
            rc = PTR_ERR(MyRunThread);
            printk(" error %d create thread_name thread", rc);
      }
      return 0;
}

//pid_t kernel_thread(int (*fn)(void *), void *arg, unsigned long flags);
//has declare no define
static int __init rcu_mod_register(void)
{
    //int pid;
    printk("=== %s entry, start create_thread\n", __func__);
    create_thread();
    create_thread_simple();
    //pid = kernel_thread(test_func, "kernel_thread create", CLONE_FS | CLONE_FILES);
    //kernel_thread(test_func,NULL,0);
    printk("=== %s finish\n", __func__);

    return 0;
}

static void __exit rcu_mod_unregister(void)
{
    printk("=== %s entry\n", __func__);
    if (!IS_ERR(MyRunThread))
    {
      kthread_stop(MyRunThread);    
      printk(" stop MyRunThread finish\n");
    }
    if (!IS_ERR(threadByCreate))
    {
      kthread_stop(threadByCreate);    
      printk(" stop threadByCreate finish\n");
    }
    printk("=== %s stop thread fininsh exit\n", __func__);
    return;
}

module_init(rcu_mod_register);
module_exit(rcu_mod_unregister);

MODULE_LICENSE("GPL");

