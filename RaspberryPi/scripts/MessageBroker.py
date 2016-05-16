#!/usr/bin/env python3
# -*- coding: utf-8 -*-

""" imports """
import threading
import queue
import time
import datetime
import random
import logging
import configparser
import os
import spidev



"""
    This is the message broker.
    It controls the SPI communication
"""
class app(threading.Thread):
    SPI_EMPTY = [0x00, 0x00, 0x00, 0x00, 0x00, 0x00]

    def __init__(self, sendQueue, recvQueue):
        threading.Thread.__init__(self)
        self.sendQueue = sendQueue
        self.recvQueue = recvQueue
        self.exit = False
        self.config = configparser.RawConfigParser()
        self.config.read('WaterCtrl.conf')

    def setup(self):
        # check permissions
        if os.geteuid() != 0:
            logging.critical(
                'need to be root for SPi access')
            exit(1)
        
        # setup SPI
        self.SPI = spidev.SpiDev()
        self.SPI.open(0, 0)
        self.SPI.max_speed_hz = self.config.getint('SPI', 'baudrate')
        
        # query config values
        self.cycleTime = self.config.getfloat('general', 'cycleTime')
        self.xfer_size = self.config.getint('SPI', 'xfer_size')
            
    def run(self):
        logging.info('Starting')
        self.setup()
        nextCycle = time.time()
        
        while True:
            # create sendbuffer
            if not self.sendQueue.empty():
                sendbuf = self.sendQueue.get()
            else:
                sendbuf = self.SPI_EMPTY
                
            # xfer data over SPI
            recvbuf = self.SPI.xfer2(sendbuf)
            
            # 6 bytes should be received
            if len(recvbuf) == 6:
                # check for error
                if recvbuf[0] != 0 and all(x==recvbuf[0] for x in recvbuf):
                    logging.error('SPI: received identical data, this may be a SPI error: %s',recvbuf)
                # only process messages not like SPI_EMPTY
                elif recvbuf != self.SPI_EMPTY:
                    # 0x01 and 0x02 are messages for messagebroker, so no forwarding
                    if recvbuf[0] == 1 or recvbuf[0] == 2:
                        logging.info("Received message: %s", recvbuf)
                    else: 
                        logging.info("Received message: %s", recvbuf)
                        self.recvQueue.put(recvbuf)

            
            # check if we should exit
            if self.exit:
                break
            
            # wait for next cycle, every second
            nextCycle = nextCycle + self.cycleTime
            sleeptime = nextCycle - time.time()
            if sleeptime >= 0.0:
                time.sleep(nextCycle - time.time())
        
        # manage exit
        self.exit_()
        logging.info('Exiting')
        return

    def exit_(self):
        # close SPI conenction
        if self.SPI is not None:
            self.SPI.close()


if __name__ == '__main__':
    print('this is not meant to run alone!')
    exit(1)
