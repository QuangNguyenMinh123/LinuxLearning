# Nokia5110 Driver
PHONY: all clean

BBB_KERNEL := /home/quangnm/Desktop/buildBBB/kernelbuildscripts/KERNEL
TOOLCHAIN := /home/quangnm/Desktop/buildBBB/kernelbuildscripts/dl/gcc-8.5.0-nolibc/arm-linux-gnueabi/bin/arm-linux-gnueabi-
C_FILE	:= Nokia5110
obj-m := $(C_FILE).o
BEAGLEBONE_PASSWORD := temppwd
BEAGLEBONE_IP := 192.168.16.106
BEAGLEBONE_HOME_PATH := debian@$(BEAGLEBONE_IP):/home/debian

first:
	scp $(C_FILE).ko $(BEAGLEBONE_HOME_PATH)

all:
	@make -C $(BBB_KERNEL) M=$(shell pwd) clean
	@make ARCH=arm CROSS_COMPILE=$(TOOLCHAIN) -C $(BBB_KERNEL) M=$(shell pwd) modules

move:
	sshpass -p $(BEAGLEBONE_PASSWORD) scp $(C_FILE).ko $(BEAGLEBONE_HOME_PATH)

clean:
	@make -C $(BBB_KERNEL) M=$(shell pwd) clean

again: clean all