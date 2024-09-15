# Brief
PWM driver for pin P9_14 GPIO50
# Tutorial
Add to buildBBB/kernelbuildscripts/KERNEL/arch/arm/boot/dts/am335x-boneblack.dts
"
/* User configuration for controlling pin P9_14 PWM */
#include <dt-bindings/pwm/pwm.h>
&am33xx_pinmux{
	pwm_1_pin: pwm_1_pin {
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
	status = "okay";
	pinctrl-names = "default";
	pinctrl-0 = <&pwm_1_pin>;
};

/{
	status = "okay";
    pwm_device {
        compatible = "p9_14_pwm_device";
        pwms = <&ehrpwm1 0 5000000 PWM_POLARITY_INVERTED>;
		pwm-names = "led-pwm"
        status = "okay";
    };
};
"