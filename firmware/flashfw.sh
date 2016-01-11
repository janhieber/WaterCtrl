#!/bin/sh

# kill openocd 
pidof openocd | xargs kill -9

sleep 1

openocd \
    -s /usr/share/openocd/scripts \
    -f interface/stlink-v2.cfg \
    -f target/stm32f1x.cfg \
    -c "reset_config none separate" \
    -c "init" \
    -c "reset halt" \
    -c "flash write_image erase $1 0x08000000" \
    -c "reset run" \
    -c shutdown
