#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import time
import spidev

SPIfreq = 1125000
SPI = None

def setup():
    #if os.geteuid() != 0:
    #    print("need to be root!")
    #    exit(1)
    global SPI
    SPI = spidev.SpiDev()
    SPI.open(0, 0)
    SPI.max_speed_hz = SPIfreq

def main():
    if SPI is None:
        print("SPI init error")
        exit(1)
    str1 = "Hello STM     \r\n"
    while True:
        sendbuf = [ord(c) for c in str1]
        resp = SPI.xfer2(sendbuf)
        resp = ''.join(chr(c) for c in resp)
        print("recv: ",  resp)
        time.sleep(0.5)

def gracefulExit():
    global SPI
    if SPI is not None:
        SPI.close()

if __name__ == '__main__':
    try:
        setup()
        main()
    except KeyboardInterrupt:
        pass
    finally:
        gracefulExit()
        exit(0)

