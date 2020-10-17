#!/bin/bash

cd data/images/gets;gphoto2 --debug --debug-logfile="./my-logfile.txt" --get-file=$1;mv $2 /home/thomas/camera_server/public;cd ../../..