#!/bin/bash

cd data/images/gets;echo $(exiv2 $1 -g Rating > ../exif.txt) > ../tmp/exif.txt;cd ../../..