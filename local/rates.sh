#!/bin/bash

cd data/images/gets;echo $(exiftool -filename -imagesize -exif:fnumber -xmp:all $1) > ../tmp/exif.txt;cd ../../..