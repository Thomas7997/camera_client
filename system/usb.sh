#!/bin/bash

/usr/local/lib/libgphoto2/print-camera-list udev-rules version 0.98 group plugdev mode 0660 > /etc/udev/rules.d/90-libgphoto2.rules
/usr/local/lib/libgphoto2/print-camera-list udev-rules version 0.98 script /home/remote > /etc/udev/rules.d/90-libgphoto2.rules
/usr/local/lib/libgphoto2/print-camera-list udev-rules version 0.98 script foobar > /etc/udev/rules.d/90-libgphoto2.rules