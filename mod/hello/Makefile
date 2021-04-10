ifneq ($(KERNELRELEASE),)

    obj-m := hello.o

else

    KERN_DIR ?= /home/ims/share/wsl2-ms-kernel-5.4.72
    PWD := $(shell pwd)

default:
	$(MAKE) -C $(KERN_DIR) M=$(PWD) modules

endif


clean:
	rm -rf *.o *~ core .depend .*.cmd *.ko *.mod.c *.mod *.order *.tmp_versions *.symvers
