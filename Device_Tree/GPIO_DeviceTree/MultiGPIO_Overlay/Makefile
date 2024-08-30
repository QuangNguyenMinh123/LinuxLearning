# Gpio Driver Legacy
BBB_KERNEL := /home/quangnm/Desktop/buildBBB/kernelbuildscripts/KERNEL
TOOLCHAIN := /home/quangnm/Desktop/buildBBB/kernelbuildscripts/dl/gcc-8.5.0-nolibc/arm-linux-gnueabi/bin/arm-linux-gnueabi-
DEVICE_TREE_MODULE := testoverlay
C_FILE_NAME	:= dt_gpio
obj-m := $(C_FILE_NAME).o
BEAGLEBONE_PASSWORD := temppwd
BEAGLEBONE_HOME_PATH := debian@192.168.0.103:/home/debian

dt: $(DEVICE_TREE_MODULE).dts
	@dtc -@ -I dts -O dtb -o $(DEVICE_TREE_MODULE).dtbo $(DEVICE_TREE_MODULE).dts

all:
	@make -C $(BBB_KERNEL) M=$(shell pwd) clean
	@make ARCH=arm CROSS_COMPILE=$(TOOLCHAIN) -C $(BBB_KERNEL) M=$(shell pwd) modules
	@dtc -@ -I dts -O dtb -o $(DEVICE_TREE_MODULE).dtbo $(DEVICE_TREE_MODULE).dts
	sshpass -p $(BEAGLEBONE_PASSWORD) scp testoverlay.dtbo $(BEAGLEBONE_HOME_PATH)
	sshpass -p $(BEAGLEBONE_PASSWORD) scp dt_gpio.ko $(BEAGLEBONE_HOME_PATH)
clean:
	@make -C $(BBB_KERNEL) M=$(shell pwd) clean
	@rm -rf $(DEVICE_TREE_MODULE).dtbo

again: clean all