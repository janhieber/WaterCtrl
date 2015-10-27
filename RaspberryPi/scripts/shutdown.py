#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import os
import sys
import time
import RPi.GPIO as gpio


def setup():
    if os.geteuid() != 0:
        print("need to be root!")
        exit(1)
    gpio.setmode(gpio.BOARD)
    gpio.setup(21, gpio.IN)

def main():
    while 1:
        if gpio.input(21) == gpio.LOW:
            os.system("reboot")
            sys.exit()
        time.sleep(0.3)

if __name__ == '__main__':
    try:
        setup()
        main()
    except KeyboardInterrupt:
        pass
    finally:
        exit(0)

