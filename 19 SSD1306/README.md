# Brief
Driver control SSD1306 using I2C2
# Tutorial
Add this to kernelbuildscripts/KERNEL/arch/arm/boot/dts/am335x-boneblack.dts
"
&i2c2 {
	status = "okay";
	ssd1306@3c{
		compatible = "QuangNM13,SSD1306";
		#address-cells = <1>;
		#size-cells = <1>;
		author = "QuangNM13";
		width = <128>;
		height = <64>;
		reg = <0x3c>;
		status = "okay";
	};
};
"