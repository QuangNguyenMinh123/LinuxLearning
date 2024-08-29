# BMP Driver
BUIL_BBB_DIR := /home/quangnm/Desktop/buildBBB/kernelbuildscripts
BBB_KERNEL := $(BUIL_BBB_DIR)/KERNEL
TOOLCHAIN := $(BUIL_BBB_DIR)/dl/gcc-8.5.0-nolibc/arm-linux-gnueabi/bin/arm-linux-gnueabi-

C_FILE_NAME	:= gpios
obj-m := $(C_FILE_NAME).o
clean:
	@make -C $(BBB_KERNEL) M=$(shell pwd) clean

all: clean
	@make ARCH=arm CROSS_COMPILE=$(TOOLCHAIN) -C $(BBB_KERNEL) M=$(shell pwd) modules

again: clean all