# Description
A simple linux kernel module to read data from BPM180 using I2c driver in beagle bone black
/*****************************************************************************************/
# Connection
P9-19 = I2C2_SCL  ------- SCL
P9-20 = I2C2_SDA  ------- SDA
/*****************************************************************************************/
# Step
Step 1: Check pin and device in I2c bus
    + Check if pin is enable in device tree     via /sys/class/i2c-adapter/i2c-<bus number>
    + Check is mux is correct in device tree    check in device tree files
Step 2: Check I2c slave address
    i2cdetect -y -r <bus> <slave address>
    bus = 0, 1, 2 according to I2c0, I2c1, I2c2
Step 3: Register device from sysfs
Step 4: Implement I2C driver
/*****************************************************************************************/
# Details
Step 1 - 2: Modify / Check device tree
Git Clone https://github.com/QuangNguyenMinh123/buildBBB.git
Navigate to kernelbuildscripts/KERNEL/arch/arm/boot/dts/am335x-boneblack.dts
Add these lines: 

&i2c2 {
	status = "okay";
	bmp180: bmp180@77 {
		compatible = "Mybmp180";
		reg = <0x77>;
		status = "okay";
	};
};

rebuild kernel: ./kernelbuildscripts/tools/rebuild.sh
add to microSD, start from step 5
reboot beagle bone black

Step 3: make all to build kernel module
		Move bmp180.ko & app.c to beagle bone black to check
		gcc to compile and build app.c
		insmod bmp180.ko
		run ./app