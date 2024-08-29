A simple linux kernel module to parse the device tree for a specific device and its properties
Compile device tree:
dtc -@ -I dts -O dtb -o testoverlay.dtbo testoverlay.dts
using overlay
=============================
Turotial
Compile testoverlay.dts to create testoverlay.dtbo file using command:
dtc -@ -I dts -O dtb -o testoverlay.dtbo testoverlay.dts

create dt_probe.ko kernel module files using makefile: make all
Copy generated dt_probe.ko & testoverlay.dtbo to beaglebone black
Use command "sudo mv testoverlay.dtbo /lib/firmware/"
use nano to modify file /boot/uEnv.txt, add these line:
enable_uboot_overlays=1
uboot_overlay_addr_0-/lib/firmware/<filename>.dtbo
In this case, <filename> is "testoverlay"
uboot_overlay_addr_0 can be uboot_overlay_addr_0,uboot_overlay_addr_1,uboot_overlay_addr_2.uboot_overlay_addr_3
poweroff beagle beaglebone
After power-up, navigate to folder /proc/device-tree/ to check if folder has name <device_name>
is created. If this folder is exist, then device tree is added successfully.
Run command "sudo insmod dt_probe.ko" and "sudo dmesg" to check returned value
