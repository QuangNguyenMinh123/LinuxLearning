INCLUDE_DIR := /usr/src/$(shell uname -r)/include/
UNAME_R :=$(shell uname -r)
KERNEL := /lib/modules/$(UNAME_R)/build
FILE_NAME :=  gpio

obj-m += $(FILE_NAME).o

all:
	sudo make -C $(KERNEL) M=$(PWD) clean
	sudo make -C $(KERNEL) M=$(PWD) modules
	sudo insmod $(FILE_NAME).ko
clean:
	sudo make -C $(KERNEL) M=$(PWD) clean
	sudo rmmod $(FILE_NAME)