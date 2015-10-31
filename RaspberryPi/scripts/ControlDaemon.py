#!/usr/bin/env python3
# -*- coding: utf-8 -*-

""" imports """
import threading
import queue
import time
import random
import logging


"""
    This is the main control daemon.
    It controls all high level functions
    like pouring and database stuff
"""
class app(threading.Thread):
    def __init__(self, queue):
        threading.Thread.__init__(self)
        self.queue = queue
        self.exit = False

    def run(self):
        logging.info('Starting')
        while True:
            # queue some data for MessageBroker
            randomData = random.choice('abcdefghij')
            self.queue.put(randomData)
            # check if we should exit
            if self.exit:
                break
            # sleep random time 0.1 - 2.0 sec
            time.sleep(random.uniform(0.1, 2.0))
        logging.info('Exiting')
        return


if __name__ == '__main__':
    print('this is not meant to run alone!')
    exit(1)
