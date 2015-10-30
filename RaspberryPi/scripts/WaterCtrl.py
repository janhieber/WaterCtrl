#!/usr/bin/env python3
# -*- coding: utf-8 -*-

""" imports """
import threading
import queue
import time
import datetime
import random
import logging
import signal
import sys


""" settings """
# log levels: CRITICAL, ERROR, WARNING, INFO, DEBUG, NOTSET
LOGLVL = logging.DEBUG




""" global vars """
# this is for stopping the threads
appExit = False



"""
This is the main control daemon.
It controls all high level functions
like pouring and database stuff
"""
class ControlDaemon(threading.Thread):
    def __init__(self, queue):
        threading.Thread.__init__(self)
        self.queue = queue

    def run(self):
        logging.info('Starting')
        global appExit
        while True:
            # queue some data for MessageBroker
            randomData = random.choice('abcdefghij')
            self.queue.put(randomData)
            # check if we should exit
            if appExit:
                break
            # sleep random time 0.1 - 2.0 sec
            time.sleep(random.uniform(0.1, 2.0))
        logging.info('Exiting')
        return



"""
This is the message broker.
It controls the SPI communication
"""
class MessageBroker(threading.Thread):
    def __init__(self, queue):
        threading.Thread.__init__(self)
        self.queue = queue

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
            if appExit:
                break
            # wait for next cycle, every second
            nextCycle = nextCycle + 1
            time.sleep(nextCycle - time.time())
        logging.info('Exiting')
        return


"""
Setup everything
"""
def setup():
    logging.basicConfig(
        level = LOGLVL,
        format = '%(asctime)s %(threadName)-15s %(message)s'
    )
    signal.signal(signal.SIGINT, signal_handler)
    signal.signal(signal.SIGUSR1, signal_handler)
    signal.signal(signal.SIGUSR2, signal_handler)

"""
Main function
"""
def main():
    # create queues for exchanging data
    sendQueue = queue.Queue()
    recvQueue = queue.Queue() # not used now
    # create threads
    t1 = ControlDaemon(sendQueue)
    t1.setName("ControlDaemon")
    t2 = MessageBroker(sendQueue)
    t2.setName("MessageBroker")
    # start threads
    t1.start()
    t2.start()
    # wait for threads to finish
    t1.join()
    t2.join()
    exit(0)

"""
Exit handler.
Tries to shutdown the app graceful.
"""
def gracefulExit():
    logging.critical("exit graceful")
    global appExit
    appExit = True

"""
Signal handler.
Handles user signals and when the app
is requested to exit, for example ctrl+c
"""
def signal_handler(signum, frame):
    if signum == signal.SIGUSR1:
        logging.info("SIGUSR1 received")
    if signum == signal.SIGUSR2:
        logging.info("SIGUSR2 received")
    if signum == signal.SIGINT:
        logging.critical("SIGINT received")
        gracefulExit()


if __name__ == '__main__':
    setup()
    logging.info('running main')
    main()
    exit(0)
