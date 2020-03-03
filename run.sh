#!/bin/bash 



export PLATFORMIO_CI_SRC=esp8266/GSM_auto/GSM_auto.ino 
export PLATFORMIO_CI_BOARDS_ARGS=--board=d1


#bash -c 'platformio ci --lib="." $PLATFORMIO_CI_BOARDS_ARGS' > file.log 2>&1

input="file.log"

while IFS= read -r line
do
  echo "{" $line "}"
done < "$input"

