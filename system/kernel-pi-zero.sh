#!/bin/bash

mkdir ~/rpi-kernel
cd ~/rpi-kernel
mkdir rt-kernel
git clone https://github.com/raspberrypi/linux.git -b rpi-4.19.y-rt
git clone https://github.com/raspberrypi/tools.git
export ARCH=arm
export CROSS_COMPILE=~/rpi-kernel/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian-x64/bin/arm-linux-gnueabihf-
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
tar czf ../rt-kernel.tgz *

# A transferer sur le dossier tmp/ de la raspberry pi
