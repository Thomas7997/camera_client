#!/bin/bash


sudo apt install git bc bison flex libssl-dev make -y
cd ~/rpi-kernel

# Linux pré-installé dans le dossier rpi-kernel

#git clone https://github.com/raspberrypi/linux.git -b rpi-4.19.y-rt
#git clone https://github.com/raspberrypi/tools.git
export ARCH=arm
export INSTALL_MOD_PATH=~/rpi-kernel/rt-kernel
export INSTALL_DTBS_PATH=~/rpi-kernel/rt-kernel

# pour zero (w)
export KERNEL=kernel
cd ~/rpi-kernel/linux
make bcmrpi_defconfig

make -j4 zImage
make -j4 modules
make -j4 dtbs
make -j4 modules_install
make -j4 dtbs_install
mkdir $INSTALL_MOD_PATH/boot
./scripts/mkknlimg ./arch/arm/boot/zImage $INSTALL_MOD_PATH/boot/$KERNEL.img
cd $INSTALL_MOD_PATH/boot
mv $KERNEL.img kernel7_rt.img
# tar czf ../rt-kernel.tgz *

sudo cp -rf * /tmp
cd /tmp
tar xzf rt-kernel.tgz
cd boot
sudo cp -rd * /boot/
cd ../lib
sudo cp -dr * /lib/
cd ../overlays
sudo cp -d * /boot/overlays
cd ..
sudo cp -d bcm* /boot/

sudo echo "kernel=kernel7_rt.img" >> /boot/config.txt
uname -r

cd /home/remote
mkdir xenomai-container
cd xenomai-container
../camera_client/system/xenomai.sh

