#!/usr/bin/python3

import os, sys, time, RPi.GPIO as gpio

gpio.setmode(gpio.BOARD)
gpio.setup(21, gpio.IN)

while 1:
	if gpio.input(21)==gpio.LOW:
		os.system("reboot")
		sys.exit()
	time.sleep(0.3)