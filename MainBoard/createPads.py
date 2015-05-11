#!/usr/bin/env python3
# -*- coding: UTF-8 -*-

INC = 2.54
XPAD = 8.89
YPAD = 96.52
RECT_WIDTH = 0.508

PAD=("<pad name=\"%i\" x=\"%f\" y=\"%f\" drill=\"1.016\" shape=\"octagon\"/>")
RECT=("<rectangle x1=\"%f\" y1=\"%f\" x2=\"%f\" y2=\"%f\" layer=\"51\"/>")


NR = 1

for i in range(0, 33):
	print(PAD % (NR, XPAD, YPAD-i*INC))
	print(RECT % (XPAD-RECT_WIDTH/2,
		(YPAD-i*INC)-RECT_WIDTH/2,
		XPAD+RECT_WIDTH/2,
		(YPAD-i*INC)+RECT_WIDTH/2))
	NR += 1

print("\r\n\r\n")


XPAD = 46.99

NR = 34

for i in range(0, 33):
	print(PAD % (NR, XPAD, (YPAD-32*INC)+i*INC))
	print(RECT % (XPAD-RECT_WIDTH/2,
		((YPAD-32*INC)+i*INC)-RECT_WIDTH/2,
		XPAD+RECT_WIDTH/2,
		((YPAD-32*INC)+i*INC)+RECT_WIDTH/2))
	NR += 1