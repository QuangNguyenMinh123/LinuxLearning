# Nokia5110 Driver
PHONY: all clean

BBB_KERNEL := /home/quangnm/Desktop/buildBBB/kernelbuildscripts/KERNEL
TOOLCHAIN := /home/quangnm/Desktop/buildBBB/kernelbuildscripts/dl/gcc-8.5.0-nolibc/arm-linux-gnueabi/bin/arm-linux-gnueabi-
C_FILE	:= Nokia5110
DEVICE_TREE := Nokia5110
obj-m := $(C_FILE).o

all:
	@make -C $(BBB_KERNEL) M=$(shell pwd) clean
	@rm -rf $(DEVICE_TREE).dtbo
	@make ARCH=arm CROSS_COMPILE=$(TOOLCHAIN) -C $(BBB_KERNEL) M=$(shell pwd) modules
	@dtc -@ -I dts -O dtb -o $(DEVICE_TREE).dtbo $(DEVICE_TREE).dts

clean:
	@make -C $(BBB_KERNEL) M=$(shell pwd) clean
	@rm -rf $(DEVICE_TREE).dtbo

again: clean all