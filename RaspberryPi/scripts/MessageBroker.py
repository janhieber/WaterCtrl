#!/usr/bin/env python3
# -*- coding: utf-8 -*-

""" imports """
import threading
import queue
import time
import datetime
import random
import logging


"""
    This is the message broker.
    It controls the SPI communication
"""
class app(threading.Thread):
    def __init__(self, queue):
        threading.Thread.__init__(self)
        self.queue = queue
        self.exit = False

    def run(self):
        logging.info('Starting')
        global appExit
        nextCycle = time.time()
        while True:
            logging.info('process queue')
            while not self.queue.empty():
                # get queued data
                recvData = self.queue.get()
                logging.info("received %s" % (recvData))
                # mark data as done
                self.queue.task_done()
            # check if we should exit
            if self.exit:
                break
            # wait for next cycle, every second
            nextCycle = nextCycle + 1
            time.sleep(nextCycle - time.time())
        logging.info('Exiting')
        return


if __name__ == '__main__':
    print('this is not meant to run alone!')
    exit(1)
