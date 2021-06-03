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
#include <linux/types.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/mm.h>
#include <linux/sched.h>
#include <linux/init.h>
#include <linux/cdev.h>
#include <asm/io.h>
#include<linux/slab.h>
#include <asm/uaccess.h>
#include <linux/version.h>
#if LINUX_VERSION_CODE > KERNEL_VERSION(3, 3, 0)
        #include <asm/switch_to.h>
#else
        #include <asm/system.h>
#endif
#include <linux/kernel.h>
#include "memdev.h"
static int mem_major = MEMDEV_MAJOR;

module_param(mem_major, int, S_IRUGO);

struct mem_dev *mem_devp; /*设备结构体指针*/

struct cdev cdev;

/*文件打开函数*/
int mem_open(struct inode *inode, struct file *filp)
{
    struct mem_dev *dev;

    /*获取次设备号*/
    int num = MINOR(inode->i_rdev);

    if (num >= MEMDEV_NR_DEVS)
            return -ENODEV;
    dev = &mem_devp[num];

    /*将设备描述结构指针赋值给文件私有数据指针*/
    filp->private_data = dev;

    return 0;
}

/*文件释放函数*/
int mem_release(struct inode *inode, struct file *filp)
{
  return 0;
}
static int memdev_mmap(struct file*filp, struct vm_area_struct *vma)
{
      struct mem_dev *dev = filp->private_data; /*获得设备结构体指针*/

      vma->vm_flags |= VM_IO;
      //vma->vm_flags |= VM_RESERVED;
      vma->vm_flags |= (VM_DONTEXPAND | VM_DONTDUMP);


      if (remap_pfn_range(vma,vma->vm_start,virt_to_phys(dev->data)>>PAGE_SHIFT, vma->vm_end - vma->vm_start, vma->vm_page_prot))
          return  -EAGAIN;

      return 0;
}

/*文件操作结构体*/
static const struct file_operations mem_fops =
{
  .owner = THIS_MODULE,
  .open = mem_open,
  .release = mem_release,
  .mmap = memdev_mmap,
};

/*设备驱动模块加载函数*/
static int memdev_init(void)
{
  int result;
  int i;

  dev_t devno = MKDEV(mem_major, 0);
  printk(KERN_INFO "mod arg mem_major:%d\n", mem_major);

  /* 静态申请设备号*/
  if (mem_major)
  {
    result = register_chrdev_region(devno, 2, "memdev");
    printk(KERN_INFO "mod static reg result:%d\n", result);
  }
  else  /* 动态分配设备号 */
  {
    result = alloc_chrdev_region(&devno, 0, 2, "memdev");
    printk(KERN_INFO "mod dynamic reg result:%d\n", result);
    mem_major = MAJOR(devno);
  }

  if (result < 0)
    return result;

  /*初始化cdev结构*/
  cdev_init(&cdev, &mem_fops);
  cdev.owner = THIS_MODULE;
  cdev.ops = &mem_fops;

  /* 注册字符设备 */
  result = cdev_add(&cdev, devno, MEMDEV_NR_DEVS);
  printk(KERN_INFO "mod cdev_add result:%d\n", result);

  /* 为设备描述结构分配内存*/
  mem_devp = kmalloc(MEMDEV_NR_DEVS * sizeof(struct mem_dev), GFP_KERNEL);

  if (!mem_devp)    /*申请失败*/
  {
    result =  - ENOMEM;
    goto fail_malloc;
  }
  memset(mem_devp, 0, sizeof(struct mem_dev));

  /*为设备分配内存*/
  for (i=0; i < MEMDEV_NR_DEVS; i++)
  {
        mem_devp[i].size = MEMDEV_SIZE;
        mem_devp[i].data = kmalloc(MEMDEV_SIZE, GFP_KERNEL);
        memset(mem_devp[i].data, 0, MEMDEV_SIZE);
  }

  printk(KERN_INFO "mod init succ\n");
  return 0;

  fail_malloc:
  unregister_chrdev_region(devno, 1);
  printk(KERN_INFO "mod init fail:%d\n", result);

  return result;
}

/*模块卸载函数*/
static void memdev_exit(void)
{
  cdev_del(&cdev);   /*注销设备*/
  kfree(mem_devp);     /*释放设备结构体内存*/
  unregister_chrdev_region(MKDEV(mem_major, 0), 2); /*释放设备号*/
}

MODULE_AUTHOR("David Xie");
MODULE_LICENSE("GPL");

module_init(memdev_init);
module_exit(memdev_exit);

