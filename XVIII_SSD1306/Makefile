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
SSD1306_SRC := $(CUR_DIR)/inc/SSD1306.c
SSD1306_INC := $(CUR_DIR)/inc


all:
	@make -C $(BBB_KERNEL) M=$(shell pwd) clean
	@make ARCH=arm CROSS_COMPILE=$(TOOLCHAIN) -C $(BBB_KERNEL) M=$(shell pwd) modules -I$(SSD1306_INC)


clean:
	@make -C $(BBB_KERNEL) M=$(shell pwd) clean

again: clean all