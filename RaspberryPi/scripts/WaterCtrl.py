#!/usr/bin/env python3
# -*- coding: utf-8 -*-

""" imports """
import threading
import time
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
    def run(self):
        logging.info('Starting')
        global appExit
        while True:
            if appExit:
                break
        logging.info('Exiting')
        return


"""
This is the message broker.
It controls the SPI communication
"""
class MessageBroker(threading.Thread):
    def run(self):
        logging.info('Starting')
        global appExit
        while True:
            if appExit:
                break
        logging.info('Exiting')
        return


def setup():
    logging.basicConfig(
        level=LOGLVL,
        format='%(asctime)s %(threadName)-15s %(message)s'
    )
    signal.signal(signal.SIGINT, signal_handler)
    signal.signal(signal.SIGUSR1, signal_handler)
    signal.signal(signal.SIGUSR2, signal_handler)

def main():
    # create threads
    t1 = ControlDaemon()
    t1.setName("ControlDaemon")
    t2 = MessageBroker()
    t2.setName("MessageBroker")
    # start threads
    t1.start()
    t2.start()
    # wait for threads to finish
    t1.join()
    t2.join()
    exit(0)

def gracefulExit():
    logging.critical("exit graceful")
    global appExit
    appExit = True

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



