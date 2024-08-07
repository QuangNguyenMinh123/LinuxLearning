# Hello world kernel module

EXTRA_CFLAGS = -Wall
obj-m = main.o

KDIR := /lib/modules/`uname -r`/build
CC := gcc

str = ""

all:
	make -C $(KDIR) M=`pwd` modules
	$(CC) -o app test_app.c
	sudo insmod main.ko
	sudo chmod 0777 /dev/m_device

clean:
	sudo rmmod main
	make -C $(KDIR) M=`pwd` clean
	rm -rf app

send:
	sudo echo $(str) > /dev/m_device

again: clean all
