obj-m += mychardev.o
all:
        make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
        sudo insmod mychardev.ko
clean:
        make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
        sudo rmmod mychardev
        sudo rm /dev/mychardev
device:
        sudo mknod /dev/mychardev c 60 0
        sudo chmod 666 /dev/mychardev