# MiscDevice Driver
PHONY: all clean

BBB_KERNEL := /home/quangnm/Desktop/buildBBB/kernelbuildscripts/KERNEL
TOOLCHAIN := /home/quangnm/Desktop/buildBBB/kernelbuildscripts/dl/gcc-8.5.0-nolibc/arm-linux-gnueabi/bin/arm-linux-gnueabi-
C_FILE	:= MiscDevice
obj-m := $(C_FILE).o

all:
	@make -C $(BBB_KERNEL) M=$(shell pwd) clean
	@make ARCH=arm CROSS_COMPILE=$(TOOLCHAIN) -C $(BBB_KERNEL) M=$(shell pwd) modules

clean:
	@make -C $(BBB_KERNEL) M=$(shell pwd) clean

again: clean all