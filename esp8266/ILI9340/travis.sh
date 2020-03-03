#!/bin/bash -x

#for i in `find . -type f -name "*.ino" | grep pubsubclient`;do


PLATFORMIO_SETTING_FORCE_VERBOSE="true"
PLATFORMIO_CI_BOARDS_ARGS="--board=d1"


for i in `find . -type f -name "*.ino"`;do

echo $i
export PLATFORMIO_CI_SRC="$i"

if [[ $PLATFORMIO_CI_BOARDS_ARGS ]]; 
	then 
		bash -c 'platformio ci $PLATFORMIO_CI_BOARDS_ARGS'; 
	else 
		platformio ci --board bbcmicrobit --board efm32pg_stk3401 --board nucleo_l152re --board frdm_kl46z --board samd21_xpro --board lpc11u35; 
fi

sleep 2

done




exit 0

sleep 10


#for i in `find . -type f -name "*.ino"`;do
#echo " export PLATFORMIO_CI_SRC="$i" PLATFORMIO_CI_BOARDS_ARGS="--board=d1" pio ci . " 
#done



