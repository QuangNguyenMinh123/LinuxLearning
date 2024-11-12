# Brief
Create builtin driver
Turn on a led right after power up
# Connection
3V3 -----
        |        |----GPIO_60
     [button]    |
        |--------|--------[1k resistor]----GND
        |
        |------[1k resistor]-----[LED]-----GND
# Tutorial
move this directory to buildBBB/kernelbuildscripts/KERNEL/drivers/

rename to BeagleBoneBuiltInDriver
cd ~buildBBB/kernelbuildscripts/KERNEL/drivers/BeagleBoneBuiltInDriver
Erase Makefile, paste:
"
EXTRA_CFLAGS = -Wall
obj-$(CONFIG_LED_MODULE) += BuiltInDriver.o
"
cd ../drivers
nano Makefile
add: "obj-$(CONFIG_LED_MODULE)		+= BeagleBoneBuiltInDriver/"
nano Kconfig
add: "source "drivers/BeagleBoneBuiltInDriver/Kconfig""
cd ../KERNEL
make menuconfig ARCH=arm    -> save to .config

rebuild kernel: kernelbuildscripts/tools/rebuildkernel.sh

if probe is failed, try to comment "enable_uboot_overlays=1" in /boot/uEnv.txt