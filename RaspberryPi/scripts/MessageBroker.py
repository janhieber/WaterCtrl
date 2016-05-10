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
        self.cycleTime = self.config.getfloat('MessageBroker', 'cycleTime')
        self.xfer_size = self.config.getint('SPI', 'xfer_size')
            
    def run(self):
        logging.info('Starting')
        self.setup()
        nextCycle = time.time()
        recvComplete = False
        
        while True:
            # process message queue
            #logging.info('process queue')
            #while not self.sendQueue.empty():
            #    # get queued data
            #    sendData = self.sendQueue.get()
            #    logging.info("sending %s" % (sendData))
            #    # mark data as done
            #    self.queue.task_done()
            
            recvComplete = False
            sendComplete = False
            recvArray = [""]
            
            while not (recvComplete and sendComplete):
                recvData = ''
                
                # create sendbuffer
                if not self.sendQueue.empty():
                    sendData = self.sendQueue.get()
                else:
                    sendData = ""
                    
                # check if send queue is now empty
                if self.sendQueue.empty():
                    sendComplete = True;
                
                # convert string to byte array
                sendbuf = [ord(c) for c in sendData]
                
                # fill with zeros
                while len(sendbuf) < 16:
                    sendbuf.append(0)
                
                # xfer data over SPI
                recvbuf = self.SPI.xfer2(sendbuf)
                
                # remove trailing zeros
                while len(recvbuf) > 0 and recvbuf[-1] == 0:
                    recvbuf.pop()
                if len(recvbuf) > 0:
                    # decode messages
                    if recvbuf[0] >= 0x01 and recvbuf[0] <= 0x13:
                        recvArray.append(''.join(chr(c) for c in recvbuf))
                    else:
                        recvArray[len(recvArray)-1] += ''.join(chr(c) for c in recvbuf)
                        
                # check if we are finnished
                if sum(recvbuf) == 0:
                    logging.info("Received message: %s", recvArray)
                    recvComplete = True
                else:
                    time.sleep(0.01)
                # check for error
                if recvComplete == False and all(x==recvbuf[0] for x in recvbuf):
                    logging.error('SPI: received identical data, this may be a SPI error')
                    recvComplete = True

            # put to queue
            for msg in recvArray:
                if len(msg) > 0:
                    self.recvQueue.put(msg.strip())
                
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
