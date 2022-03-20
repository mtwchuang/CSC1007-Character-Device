obj-m   := charDev.o

KERNELDIR ?= /lib/modules/$(shell uname -r)/build
PWD       := $(shell pwd)

all:
	$(MAKE) -C $(KERNELDIR) M=$(PWD)
	sudo insmod charDev.ko
	sudo mknod /dev/charDev 235 0
	sudo chmod 666 /dev/charDev
clean:
	sudo rmmod charDev
	sudo rm /dev/charDev
	rm -rf *.o *~ core .depend .*.cmd *.ko *.mod.c .tmp_versions *.symvers *.order
	