# Brief
This is guideline instructing you run QT QML application on beagle bone black and displaying it into display via BBB's HDMI port
In this case is car dashboard
# Step 1: Install neccessary packages
sudo apt update
sudo apt install read-edid xserver-xorg-video-fbdev x11-xserver-utils -y
sudo apt-get -y update
sudo apt-get -y upgrade
sudo apt-get -y install build-essential
sudo apt-get -y install linux-headers-$(uname -r)

sudo apt install -y build-essential g++ libxcb-xinerama0 libx11-dev libxext-dev libxt-dev libxrandr-dev libglib2.0-dev libssl-dev \
    libfreetype6-dev libjpeg-dev libpng-dev libsqlite3-dev libglib2.0-dev libudev-dev libinput-dev \
    libxkbcommon-dev libssl-dev libicu-dev
sudo apt install -y qt5-qmake qtbase5-dev qtchooser qt5-qmake-bin qtbase5-dev-tools
sudo apt-get install -y qtquickcontrols2 qtquickcontrols2-dev
sudo apt-get install -y qtdeclarative-dev

sudo apt-get install -y qml-module-qtquick-layouts qml-module-qtquick-window2 qml-module-qtquick2 \
     qml-module-qtquick-controls qml-module-qtquick-extras qml-module-qtquick-privatewidgets \
    qml-module-qtquick-controls2 qml-module-qtquick-shapes

sudo apt install -y cmake
sudo apt install -y xvfb

sudo apt-get install -y qtdeclarative5-dev
sudo apt-get install -y qt5-qtmultimedia qt5-qtwebengine qt5-qtsensors
sudo apt-get install -y qt5-qmlscene
sudo apt-get install -y libqt5serialport5-dev

# Setup environment with sudo su privilege
Xvfb :0 -screen 0 1280x1024x24 &
export DISPLAY=:0
export QT_QPA_PLATFORM_PLUGIN_PATH=/usr/lib/arm-linux-gnueabihf/qt5/plugins/platforms
export QT_QPA_PLATFORM=linuxfb

# Compile and run on beagle bone black
Copy folder QT_Display_Cmake to beagle bone black
    `cd QT_Display_Cmake` to go to project
    `mkdir build` to create build folder
    `cd build` to go to build folder
    `cmake ..` run cmake and all generated files will be located in build folder
    `make` to compile code
    `./QT_Display_Cmake` to run

# How to search qtquick packages
apt-cache search qtquick
sudo apt-get install -y <package name>