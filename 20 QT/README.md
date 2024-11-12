# Brief
Learn how to create and modify QT project

# Dependency
Install QT framework: sudo apt install qtcreator
Dependencies:
sudo apt install libglx-dev libgl1-mesa-dev libxcb-cursor0
sudo apt install xz-utils
sudo apt-get install qtbase5-dev
sudo apt-get install qtdeclarative5-dev
sudo apt-get install qtmultimedia5-dev
sudo apt-get install qtquickcontrols2-5-dev
# Issues
1.1 No valid Kits found
    `sudo apt-get install qt5-default`
Go to "Tools>Options"
Select the Qt Versions combobox and select and click Add
Then find the qmake file in the location of step 2. Here `/usr/lib/x86_64-linux-gnu/qt5/bin/` here you have the `qmake` file for qt5. Open it, click Apply
Go to "Kits" combobox. Select "Desktop(by default)" Then scroll down to the button to select Qt version: and list down to select the version you just add.
Then apply all. Check your compiler and debugger and it's ok.
