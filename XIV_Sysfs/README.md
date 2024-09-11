# Brief
control read/write GPIO 60 using custom Sysfs
# Connection

3V3 -----
        |        |----GPIO_60
     [button]    |
        |--------|--------[1k resistor]----GND
        |
        |------[1k resistor]-----[LED]-----GND
# Tutorial
make all -> move Sysfs.ko to BBB and insmod
echo in > /sys/bbb_gpio/direction
compile test.c and run app