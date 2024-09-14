# Brief
PWM driver for pin P9_14 GPIO50
# Tutorial
Add to buildBBB/kernelbuildscripts/KERNEL/arch/arm/boot/dts/am335x-boneblack.dts
"
/* User configuration for controlling pin P9_14 PWM */
&am33xx_pinmux{
	ehrpwm1_pins: ehrpwm1_pins {
		pinctrl-single,pins = <
			AM33XX_IOPAD(0x848, PIN_OUTPUT | MUX_MODE6)		/* P9_14 */
			AM33XX_IOPAD(0x84c, PIN_OUTPUT | MUX_MODE6)		/* P9_16 */
		>;
	};
};
/* Node epwmss1 contains ehrpwm1 so epwmss1 must be enabled first */
&epwmss1{
    status = "okay";
};

&ehrpwm1{
	MyPWM {
		compatible = "P9_14_USERPWM";
		author = "QuangNM13";
		pinctrl-names = "default", "pwm";
    	pinctrl-0 = <&ehrpwm1_pins>;
    	status = "okays";
	};
};
"