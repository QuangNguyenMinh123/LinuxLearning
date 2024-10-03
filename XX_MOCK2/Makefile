# ILI9341 Driver
PHONY: all clean

BBB_KERNEL := /home/quangnm/Desktop/buildBBB/kernelbuildscripts/KERNEL
TOOLCHAIN := /home/quangnm/Desktop/buildBBB/kernelbuildscripts/dl/gcc-8.5.0-nolibc/arm-linux-gnueabi/bin/arm-linux-gnueabi-
Driver_FILE	:= ILI9341_driver
obj-m := ILI9341_driver.o
ILI9341_driver-objs := ILI9341.o ILI9341_ctrl.o ILI9341_GUI.o

BEAGLEBONE_PASSWORD := temppwd
BEAGLEBONE_IP := 192.168.16.101
BEAGLEBONE_HOME_PATH := debian@$(BEAGLEBONE_IP):/home/debian

first:
	scp $(Driver_FILE).ko $(BEAGLEBONE_HOME_PATH)

all:
	@make ARCH=arm CROSS_COMPILE=$(TOOLCHAIN) -C $(BBB_KERNEL) M=$(shell pwd) modules

move:
	sshpass -p $(BEAGLEBONE_PASSWORD) scp $(Driver_FILE).ko $(BEAGLEBONE_HOME_PATH)

clean:
	@make -C $(BBB_KERNEL) M=$(shell pwd) clean

again: clean all