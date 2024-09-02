# Gpio Driver Legacy
BBB_KERNEL := /home/quangnm/Desktop/buildBBB/kernelbuildscripts/KERNEL
TOOLCHAIN := /home/quangnm/Desktop/buildBBB/kernelbuildscripts/dl/gcc-8.5.0-nolibc/arm-linux-gnueabi/bin/arm-linux-gnueabi-
DEVICE_TREE_MODULE := GPIO_Overlay
C_FILE_NAME	:= GPIO_Overlay
obj-m := $(C_FILE_NAME).o
BEAGLEBONE_PASSWORD := temppwd
BEAGLEBONE_IP := 192.168.16.103
BEAGLEBONE_HOME_PATH := debian@$(BEAGLEBONE_IP):/home/debian

dt: $(DEVICE_TREE_MODULE).dts
	@dtc -@ -I dts -O dtb -o $(DEVICE_TREE_MODULE).dtbo $(DEVICE_TREE_MODULE).dts

all:
	make -C $(BBB_KERNEL) M=$(shell pwd) clean
	rm -rf $(DEVICE_TREE_MODULE).dtbo
	make ARCH=arm CROSS_COMPILE=$(TOOLCHAIN) -C $(BBB_KERNEL) M=$(shell pwd) modules
	dtc -@ -I dts -O dtb -o $(DEVICE_TREE_MODULE).dtbo $(DEVICE_TREE_MODULE).dts
	sshpass -p $(BEAGLEBONE_PASSWORD) scp $(DEVICE_TREE_MODULE).dtbo $(BEAGLEBONE_HOME_PATH)
	sshpass -p $(BEAGLEBONE_PASSWORD) scp $(C_FILE_NAME).ko $(BEAGLEBONE_HOME_PATH)

clean:
	@make -C $(BBB_KERNEL) M=$(shell pwd) clean
	@rm -rf $(DEVICE_TREE_MODULE).dtbo

again: clean all