# Sysfs Driver
PHONY: all clean move

BBB_KERNEL := /home/quangnm/Desktop/buildBBB/kernelbuildscripts/KERNEL
TOOLCHAIN := /home/quangnm/Desktop/buildBBB/kernelbuildscripts/dl/gcc-8.5.0-nolibc/arm-linux-gnueabi/bin/arm-linux-gnueabi-
C_FILE	:= Sysfs
obj-m := $(C_FILE).o
BEAGLEBONE_PASSWORD := temppwd
BEAGLEBONE_IP := 192.168.16.103
BEAGLEBONE_HOME_PATH := debian@$(BEAGLEBONE_IP):/home/debian

all:
	@make -C $(BBB_KERNEL) M=$(shell pwd) clean
	@make ARCH=arm CROSS_COMPILE=$(TOOLCHAIN) -C $(BBB_KERNEL) M=$(shell pwd) modules

clean:
	@make -C $(BBB_KERNEL) M=$(shell pwd) clean

move:
	sshpass -p $(BEAGLEBONE_PASSWORD) scp $(C_FILE).ko $(BEAGLEBONE_HOME_PATH)

again: clean all