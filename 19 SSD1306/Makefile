# SSD1306 Driver
.PHONY: all clean

CUR_DIR := .
INC_DIR := $(CUR_DIR)/inc
SRC_DIR := $(CUR_DIR)/src
BBB_KERNEL := /home/quangnm/Desktop/buildBBB/kernelbuildscripts/KERNEL
TOOLCHAIN := /home/quangnm/Desktop/buildBBB/kernelbuildscripts/dl/gcc-8.5.0-nolibc/arm-linux-gnueabi/bin/arm-linux-gnueabi-
C_FILE	:= SSD1306_Driver
DEVICE_TREE := SSD1306_Driver
obj-m := $(C_FILE).o
#Included header
lib-y    := SSD1306.o
BEAGLEBONE_PASSWORD := temppwd
BEAGLEBONE_IP := 192.168.16.101
BEAGLEBONE_HOME_PATH := debian@$(BEAGLEBONE_IP):/home/debian

first:
	scp $(C_FILE).ko $(BEAGLEBONE_HOME_PATH)

all:
	@make -C $(BBB_KERNEL) M=$(shell pwd) clean
	@make -I$(INC_DIR) ARCH=arm CROSS_COMPILE=$(TOOLCHAIN) -C $(BBB_KERNEL) M=$(shell pwd) modules 

move:
	sshpass -p $(BEAGLEBONE_PASSWORD) scp $(C_FILE).ko $(BEAGLEBONE_HOME_PATH)

clean:
	@make -C $(BBB_KERNEL) M=$(shell pwd) clean

again: clean all