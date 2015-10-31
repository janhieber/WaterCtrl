#!/usr/bin/env python3
# -*- coding: utf-8 -*-

""" imports """
import queue
import logging
import signal
import sys
import ControlDaemon
import MessageBroker

""" settings """
# log levels: CRITICAL, ERROR, WARNING, INFO, DEBUG, NOTSET
LOGLVL = logging.DEBUG
#LOGFILE = 'WaterCtrl.log' # for file log
LOGFILE = None # for console log



""" global vars """
thread1 = None
thread2 = None



"""
    Setup everything
"""
def setup():
    # setup logging
    logging.basicConfig(
        filename=LOGFILE,
        level = LOGLVL,
        format = '%(asctime)s %(levelname)-8s [%(threadName)-13s] %(message)s'
    )
    # setup signal traps
    signal.signal(signal.SIGINT, signal_handler)
    signal.signal(signal.SIGUSR1, signal_handler)
    signal.signal(signal.SIGUSR2, signal_handler)
    # some empty loglines on start
    logging.critical('=====================================')
    logging.critical('=====================================')
    logging.critical('=====================================')


"""
    Main function
"""
def main():
    # create queues for exchanging data
    sendQueue = queue.Queue()
    recvQueue = queue.Queue() # not used now
    # create threads
    global thread1
    global thread2
    thread1 = ControlDaemon.app(sendQueue)
    thread1.setName('ControlDaemon')
    thread2 = MessageBroker.app(sendQueue)
    thread2.setName('MessageBroker')
    # start threads
    thread1.start()
    thread2.start()
    # wait for threads to finish
    thread1.join()
    thread2.join()
    exit(0)

"""
    Exit handler.
    Tries to shutdown the app graceful.
"""
def gracefulExit():
    logging.critical('exit graceful')
    global thread1
    global thread2
    thread1.exit = True
    thread2.exit = True

"""
    Signal handler.
    Handles user signals and when the app
    is requested to exit, for example ctrl+c
"""
def signal_handler(signum, frame):
    if signum == signal.SIGUSR1:
        logging.info('SIGUSR1 received')
    if signum == signal.SIGUSR2:
        logging.info('SIGUSR2 received')
    if signum == signal.SIGINT:
        logging.critical('SIGINT received')
        gracefulExit()


if __name__ == '__main__':
    setup()
    logging.info('running main')
    main()
    logging.info('exit')
    exit(0)
