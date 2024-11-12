# Brief
Driver control Nokia5110 using SPI0
# Tutorial
Add this to kernelbuildscripts/KERNEL/arch/arm/boot/dts/am335x-boneblack.dts and rebuild BBB
"
/* Nokia5110 DT */
&am33xx_pinmux{
	spi0_pinmux: spi0_pinmux {
		pinctrl-single,pins = <
			AM33XX_IOPAD(0x095c, PIN_OUTPUT_PULLUP | INPUT_EN | MUX_MODE0)	/* P9_17: SPI0_CS0 */
			AM33XX_IOPAD(0x0954, PIN_OUTPUT_PULLUP | INPUT_EN | MUX_MODE0)	/* P9_21: SPI0_D0 */
			AM33XX_IOPAD(0x0958, PIN_OUTPUT_PULLUP | INPUT_EN | MUX_MODE0)	/* P9_18: SPI0_D1 */
			AM33XX_IOPAD(0x0950, PIN_OUTPUT_PULLUP | INPUT_EN | MUX_MODE0)	/* P9_22: SPI0_SCLK */
			AM33XX_IOPAD(0x089c, PIN_OUTPUT | INPUT_EN | MUX_MODE7)	/* P8_9: GPIO69 */
			AM33XX_IOPAD(0x0834, PIN_OUTPUT | INPUT_EN | MUX_MODE7)	/* P8_9: GPIO45 */
		>;
	};
};

&spi0 {
	status = "okay";
	ti,pindir-d0-out-d1-in = <1>;		/* convert SPI_D0 = MOSI, SPI_D1 = MISO*/
	Nokia5110: Nokia5110@0 {
		compatible = "QuangNM13,Nokia5110";
		author = "QuangNM13";
		pinctrl-names = "default";
		pinctrl-0 = <&spi0_pinmux>;
        spi-max-frequency = <4000000>;
		reset-gpio = <&gpio2 5 0>;
		commandData-gpio = <&gpio1 13 0>;
		reg = <0>;
		status = "okay";
	};
};
"

make clean all move to move Nokia5110.ko driver to BBB
insmod Nokia5110.ko
compile and run test.c to move cursor to { x = 30 , y = 3}
echo "<your string>" > /proc/nokia5110   To display <your string> to nokia5110