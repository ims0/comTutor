#ifneq 括号内不能有空格 
ifneq ($(KERNELRELEASE),)
	obj-m := netf.o
	netf-objs := netfilter.o 
	br:= first
else
	KERNEL_DIR ?= /usr/src/kernels/$(shell uname -r)
	PWD := $(shell pwd)
	br:= second
endif
CFLAGS := -std=C99

default:
	@echo $(br)
	@echo KERNELRELEASE:$(KERNELRELEASE)
	@echo KERNEL_DIR:$(KERNEL_DIR)
	$(MAKE) -C $(KERNEL_DIR) M=$(PWD) modules

clean:
	rm -rf *.o *~ core .depend .*.cmd *.ko *.mod.c *.mod *.order *.tmp_versions *.symvers

.PHONY:clean


#是在内核源码的顶层Makefile中定义的一个变量，在第一次(刚开始)读取执行此Makefile时，KERNELRELEASE没有被定义（为空），所以make将读取执行else之后的内容。如果make的目标是clean，直接执行clean操作，然后结束。当make的目标为all时，-C(KDIR)指明跳转到内核源码目录下读取那里的Makefile，M=(PWD)表明然后返回到当前目录继续读入、执行当前的Makefile。当从内核源码目录返回时，KERNELRELEASE已被被定义，kbuild也被启动去解析kbuild语法的语句，make将继续读取else之前的内容（指 obj-m :=hello.o）。else之前的内容为kbuild语法的语句,指明模块源码中各文件的依赖关系，以及要生成的目标模块名。

#原文链接：https://blog.csdn.net/u012247418/article/details/83684214
