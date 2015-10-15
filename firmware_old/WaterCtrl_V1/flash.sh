#!/bin/bash

VER=1
OPENOCD_SCRIPTS="/usr/share/openocd/scripts"

if [[ "$1" == "debug" ]]
then
    FILE="Debug/WaterCtrl_V1.bin"
elif [[ "$1" == "release" ]]
then
    FILE="Release/WaterCtrl_V1.bin"
fi

if [[ -z $FILE ]]
then
    echo -e "Flashscript V$VER"
    echo -e "usage: flash.sh <option>"
    echo -e "\r\noptions:"
    echo -e "\tdebug      - flash debug firmware"
    echo -e "\trelease    - flash release firmware"
    echo -e "\r\n\r\n"
    exit 1
fi

/usr/bin/openocd \
    -s "$OPENOCD_SCRIPTS" \
    -f interface/stlink-v2.cfg \
    -f target/stm32f1x.cfg \
    -c "transport select hla_swd" \
    -c "reset_config none separate" \
    -c "init" \
    -c "reset halt" \
    -c "sleep 100" \
    -c "wait_halt 2" \
    -c "flash write_image erase $FILE 0x08000000" \
    -c "sleep 100" \
    -c "reset run" \
    -c shutdown

exit 0