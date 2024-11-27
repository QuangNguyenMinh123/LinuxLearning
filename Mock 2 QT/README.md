
# Setup
sudo su
Xvfb :0 -screen 0 1280x1024x24 &
export DISPLAY=:0
export QT_QPA_PLATFORM_PLUGIN_PATH=/usr/lib/arm-linux-gnueabihf/qt5/plugins/platforms
export QT_QPA_PLATFORM=linuxfb
# find QT package
apt-cache search qtquick
