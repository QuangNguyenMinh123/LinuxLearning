this folder contains source code for running GPIO and helloworld.
Using native compile in BBB itself
=========================================
Trouble shoot:
Install library for native compile:
    sudo apt-get install linux-headers-$(uname -r)
    sudo apt-get install linux-headers-generic
    sudo apt-get install build-essential                # install makefile...

If this error occurs "*** No rule to make target 'arch/arm/kernel/module.lds', needed by '/home/debian/gpio/gpio.ko'.  Stop."
create file "module.lds" in directory "/usr/src/<shell uname -r>/arch/arm/kernel/"
add these lines:

"
/* SPDX-License-Identifier: GPL-2.0 */
SECTIONS {
	.plt : { BYTE(0) }
	.init.plt : { BYTE(0) }
}
"
Problem shall be solved
