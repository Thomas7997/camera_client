#/bin/bash

build_root=/home/remote/x-mods
staging_dir=/home/remote/x-mods-install
xenomai_root=/home/remote/xenomai-3.1

# Il y aura plus tard un moyen plus rapide en copiant les images, noyaux et modules de production respectivement de linux, PREEMPT_RT et Xenomai Mercury

mkdir $build_root/xenomai && cd $build_root/xenomai
$xenomai_root/configure CFLAGS="-march=armv4t" LDFLAGS="-march=armv4t" \
  --build=i686-pc-linux-gnu --host=arm-none-linux-gnueabi- --with-core=cobalt
make DESTDIR=$staging_dir install
