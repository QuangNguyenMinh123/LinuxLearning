I - X: Linux general
XI - XX: Linux driver
===================================================
# Connection:
Please refer OpenBoard/Schematic.pdf
===================================================
decompile dtb(o) to dts
dtc -I dtb -O dts -o temp.dts GPIO_Overlay.dtbo
dtc -I dtb -O dts -o am335x-boneblack.dts am335x-boneblack.dtb
compiling
dtc -I dts -O dtb tmp.dts -o tmp.dtbo
