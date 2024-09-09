# Brief
A simple linux kernel module to parse the device tree for GPIO
This exmaple doesn't add devicev to device tree at build time 
Device is added at run-time
=============================
# Turotial
How pinmux works:
cat /sys/kernel/debug/pinctrl/44e10800.pinmux-pinctrl-single/pins
when start up:
    pin 28 (PIN28) 44e10870 00000037 pinctrl-single
    pin 29 (PIN29) 44e10874 00000037 pinctrl-single
    pin 16 (PIN16) 44e10840 00000027 pinctrl-single
    pin 87 (PIN87) 44e1095c 00000030 pinctrl-single
    pin 85 (PIN85) 44e10954 00000020 pinctrl-single
install default
    pin 28 (PIN28) 44e10870 0000002f pinctrl-single
    pin 29 (PIN29) 44e10874 0000002f pinctrl-single
    pin 16 (PIN16) 44e10840 0000002f pinctrl-single
    pin 85 (PIN85) 44e10954 0000002f pinctrl-single
    pin 87 (PIN87) 44e1095c 0000002f pinctrl-single
install but pinctrl-names is wrong:
    pin 28 (PIN28) 44e10870 0000002f pinctrl-single
    pin 29 (PIN29) 44e10874 0000002f pinctrl-single
    pin 16 (PIN16) 44e10840 0000002f pinctrl-single
    pin 85 (PIN85) 44e10954 0000002f pinctrl-single
    pin 87 (PIN87) 44e1095c 0000002f pinctrl-single
install gpio
    pin 28 (PIN28) 44e10870 00000028 pinctrl-single
    pin 29 (PIN29) 44e10874 00000028 pinctrl-single
    pin 16 (PIN16) 44e10840 00000028 pinctrl-single
    pin 85 (PIN85) 44e10954 00000028 pinctrl-single
    pin 87 (PIN87) 44e1095c 00000028 pinctrl-single