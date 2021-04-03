#!/bin/bash
# Ajout des paquets non installés

sudo apt-get install flex bison libssl-dev -y
mkdir ~/rpi-kernel
cd ~/rpi-kernel
mkdir rt-kernel
git clone https://github.com/raspberrypi/linux.git -b rpi-4.19.y-rt
git clone https://github.com/raspberrypi/tools.git
export ARCH=arm
#export CROSS_COMPILE=~/rpi-kernel/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian-x64/bin/arm-linux-gnueabihf-
export INSTALL_MOD_PATH=~/rpi-kernel/rt-kernel
export INSTALL_DTBS_PATH=~/rpi-kernel/rt-kernel
export KERNEL=kernel7l
cd ~/rpi-kernel/linux
make bcm2711_defconfig
make -j4 zImage
make -j4 modules
make -j4 dtbs
make -j4 modules_install
make -j4 dtbs_install
mkdir $INSTALL_MOD_PATH/boot
./scripts/mkknlimg ./arch/arm/boot/zImage $INSTALL_MOD_PATH/boot/$KERNEL.img
cd $INSTALL_MOD_PATH/boot
cd ..
mv $KERNEL.img kernel7_rt.img
sudo cp -rf * /tmp

cd /tmp
cd boot
sudo cp -rd * /boot/
cd ../lib
sudo cp -dr * /lib/
#cd ../overlays
#sudo cp -d * /boot/overlays
#cd ..
#sudo cp -d bcm* /boot
