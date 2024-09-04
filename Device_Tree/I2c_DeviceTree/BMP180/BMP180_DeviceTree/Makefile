# I2c bmp180 Driver
PHONY: all move clean
BBB_KERNEL := /home/quangnm/Desktop/buildBBB/kernelbuildscripts/KERNEL
TOOLCHAIN := /home/quangnm/Desktop/buildBBB/kernelbuildscripts/dl/gcc-8.5.0-nolibc/arm-linux-gnueabi/bin/arm-linux-gnueabi-
C_FILE_NAME	:= bmp180_DeviceTree
obj-m := $(C_FILE_NAME).o
BEAGLEBONE_PASSWORD := temppwd
BEAGLEBONE_IP := 192.168.16.103
BEAGLEBONE_HOME_PATH := debian@$(BEAGLEBONE_IP):/home/debian

clean:
	@make -C $(BBB_KERNEL) M=$(shell pwd) clean

build:
	@make ARCH=arm CROSS_COMPILE=$(TOOLCHAIN) -C $(BBB_KERNEL) M=$(shell pwd) modules

move:
	@sshpass -p $(BEAGLEBONE_PASSWORD) scp $(C_FILE_NAME).ko $(BEAGLEBONE_HOME_PATH)

all: clean build