I - X: Linux general
XI - ..: Linux driver
===================================================
# Connection:
LED1  ---------------------- GPIO0_30
LED2  ---------------------- GPIO0_31
LED3  ---------------------- GPIO0_48
LED4  ---------------------- GPIO0_05
LED5  ---------------------- GPIO0_03

#PWM

#ILI9341


#BMP180
SCL  ---------------------- I2C2-SCL
SDA  ---------------------- I2C2-SDA

#ADC

===================================================
decompile dtb(o) to dts
dtc -I dtb -O dts -o temp.dts GPIO_Overlay.dtbo
dtc -I dtb -O dts -o am335x-boneblack.dts am335x-boneblack.dtb
compiling
dtc -I dts -O dtb tmp.dts -o tmp.dtbo
