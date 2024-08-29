# Brief
A simple linux kernel module to parse the device tree for multiple GPIO
Leds are added at build time
=============================
# Connection
Refer ../../README.md
LED1  ---------------------- GPIO0_30
LED2  ---------------------- GPIO0_31
LED3  ---------------------- GPIO0_48
LED4  ---------------------- GPIO0_05
LED5  ---------------------- GPIO0_03
=============================
# Turotial
&gpio0 {
	status = "okay";
	led1: led1@0 {
		compatible = "MyLED1";
		reg = <30>;
		status = "okay";
	};
	led2: led2@0 {
		compatible = "MyLED2";
		reg = <31>;
		status = "okay";
	};
	led4: led4@0 {
		compatible = "MyLED4";
		reg = <05>;
		status = "okay";
	};
	led5: led5@0 {
		compatible = "MyLED5";
		reg = <03>;
		status = "okay";
	};
};

&gpio1 {
	status = "okay";
	led3: led3@0 {
		compatible = "MyLED3";
		reg = <17>;
		status = "okay";
	};
};