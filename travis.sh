#!/bin/bash

#for i in `find projects -type f -name "*.ino" | grep pubsubclient`;do

for i in `find esp8266 -type f -name "*.ino"`;do
echo "    - PLATFORMIO_CI_SRC="$i" PLATFORMIO_CI_BOARDS_ARGS="--board=d1"" 
done



