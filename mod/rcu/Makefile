#ifneq 括号内不能有空格 
ifneq ($(KERNELRELEASE),)
	obj-m := rcumod.o
	rcumod-objs := rcu.o
else
	KERNEL_DIR ?= /lib/modules/$(shell uname -r)/build
	PWD := $(shell pwd)
endif


all:
	@echo KERNEL_DIR:$(KERNEL_DIR)
	$(MAKE) -C $(KERNEL_DIR) M=$(PWD) modules

clean:
	rm -rf *.o *~ core .depend .*.cmd *.ko *.mod.c *.mod *.order *.tmp_versions *.symvers test

.PHONY:clean


