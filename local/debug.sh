#!/bin/bash

console_user=`cat /var/run/console/console.lock` 

if [ -z "$console_user" ] ; then
  exit 1 
fi

if [ -z "$HAL_PROP_USB_BUS_NUMBER" -o -z "$HAL_PROP_USB_LINUX_DEVICE_NUMBER" ] ; then
  exit 2 
fi

if [ $HAL_PROP_USB_BUS_NUMBER -lt 0 -o  $HAL_PROP_USB_LINUX_DEVICE_NUMBER -lt 0 ] ; then
  exit 3 
fi

bus_num=`printf %.3u $HAL_PROP_USB_BUS_NUMBER`
dev_num=`printf %.3u $HAL_PROP_USB_LINUX_DEVICE_NUMBER`

NUM_TRIES_LEFT=5
while [ $NUM_TRIES_LEFT -ge 0 ] && [ ! -c /dev/bus/usb/$bus_num/$dev_num ]; do
        sleep 1
        NUM_TRIES_LEFT=$(($NUM_TRIES_LEFT - 1))
done
[ -c /dev/bus/usb/$bus_num/$dev_num ] || exit 4

chown $console_user /dev/bus/usb/$bus_num/$dev_num 

NUM_TRIES_LEFT=5
while [ $NUM_TRIES_LEFT -ge 0 ] && [ ! -f /proc/bus/usb/$bus_num/$dev_num ]; do
        sleep 1
        NUM_TRIES_LEFT=$(($NUM_TRIES_LEFT - 1))
done
[ -f /proc/bus/usb/$bus_num/$dev_num ] || exit 5

chown $console_user /proc/bus/usb/$bus_num/$dev_num
