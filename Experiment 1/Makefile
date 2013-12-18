TARGET=process
obj-m+=$(TARGET).o
PWD=$(shell pwd)
kernel_path=/usr/src/linux-headers-$(shell uname -r)
all:
        make -C $(kernel_path) M=$(PWD) modules
clean:
        make -C $(kernel_path) M=$(PWD) clean