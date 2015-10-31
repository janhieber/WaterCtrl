#!/usr/bin/env python3
# -*- coding: utf-8 -*-

""" imports """
import threading
import queue
import time
import datetime
import random
import logging
#import spidev


""" settings """
SPIfreq = 1125000




""" global vars """



"""
    This is the message broker.
    It controls the SPI communication
"""
class app(threading.Thread):
    def __init__(self, queue):
        threading.Thread.__init__(self)
        self.queue = queue
        self.exit = False
        self.SPI = None

    def setup(self):
        """
        # check permissions
        if os.geteuid() != 0:
            logging.critical(
                'need to be root for SPi access')
            exit(1)
        # setup SPI
        self.SPI = spidev.SpiDev()
        self.SPI.open(0, 0)
        self.SPI.max_speed_hz = SPIfreq
        """
        pass
            
    def run(self):
        logging.info('Starting')
        self.setup()
        nextCycle = time.time()
        str1 = "Hello STM     \r\n"
        while True:
            logging.info('process queue')
            while not self.queue.empty():
                # get queued data
                recvData = self.queue.get()
                logging.info("received %s" % (recvData))
                # mark data as done
                self.queue.task_done()
            """
            sendbuf = [ord(c) for c in str1]
            resp = self.SPI.xfer2(sendbuf)
            resp = ''.join(chr(c) for c in resp)
            """
            # check if we should exit
            if self.exit:
                break
            # wait for next cycle, every second
            nextCycle = nextCycle + 1
            time.sleep(nextCycle - time.time())
        if self.SPI is not None:
            self.SPI.close()
        logging.info('Exiting')
        return


if __name__ == '__main__':
    print('this is not meant to run alone!')
    exit(1)
