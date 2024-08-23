# Gpio Driver Legacy
BBB_KERNEL := /home/quangnm/Desktop/buildBBB/kernelbuildscripts/KERNEL
TOOLCHAIN := /home/quangnm/Desktop/buildBBB/kernelbuildscripts/dl/gcc-8.5.0-nolibc/arm-linux-gnueabi/bin/arm-linux-gnueabi-
DEVICE_TREE_MODULE := testoverlay
C_FILE_NAME	:= dt_gpio
obj-m := $(C_FILE_NAME).o

devite_tree: $(DEVICE_TREE_MODULE).dts
	@dtc -@ -I dts -O dtb -o $(DEVICE_TREE_MODULE).dtbo $(DEVICE_TREE_MODULE).dts

all:
	@make ARCH=arm CROSS_COMPILE=$(TOOLCHAIN) -C $(BBB_KERNEL) M=$(shell pwd) modules
	@dtc -@ -I dts -O dtb -o $(DEVICE_TREE_MODULE).dtbo $(DEVICE_TREE_MODULE).dts

clean:
	@make -C $(BBB_KERNEL) M=$(shell pwd) clean
	@rm -rf $(DEVICE_TREE_MODULE).dtbo

again: clean all