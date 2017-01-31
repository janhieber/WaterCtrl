#!/usr/bin/env python3
# -*- coding: utf-8 -*-

""" imports """
import queue
import logging
import signal
import sys
import configparser
import ControlDaemon
import MessageBroker
import WebService

from flask import Flask, request

""" global vars """
thread1 = None
thread2 = None
thread3 = None
config = None

server = Flask(__name__)

"""
    Setup everything
"""
def setup():
    # read config
    config = configparser.RawConfigParser()
    config.read('WaterCtrl.conf')

    # setup logging
    logging.basicConfig(
        filename = config.get('general', 'logfile'),
        level = config.getint('general', 'loglevel'),
        format = '%(asctime)s %(levelname)-8s [%(threadName)-13s] %(message)s'
    )

    # setup signal traps
    signal.signal(signal.SIGINT, signal_handler)
    signal.signal(signal.SIGUSR1, signal_handler)
    signal.signal(signal.SIGUSR2, signal_handler)

    # create queues for exchanging data
    sendQueue = queue.Queue()
    recvQueue = queue.Queue() # not used now

    # create threads
    global thread1
    global thread2
    global thread3
    thread1 = ControlDaemon.app(sendQueue, recvQueue)
    thread1.setName('ControlDaemon')
    thread2 = MessageBroker.app(sendQueue, recvQueue)
    thread2.setName('MessageBroker')
    thread3 = WebService.app(server,sendQueue=sendQueue)
    thread3.setName('WebService')

    thread1.start()
    thread2.start()
    thread3.start()
    # some empty loglines on start
    logging.critical('STARTUP done\n\n\n\n\n')

"""
    Main function
"""
def main():
    logging.info("main function")

"""
    Exit handler.
    Tries to shutdown the app graceful.
"""
def gracefulExit():
    logging.critical('exit graceful')
    # signal threads to exit
    global thread1
    global thread2
    thread1.exit = True
    thread2.exit = True

    func = request.environ.get('werkzeug.server.shutdown')
    if func is None:
        raise RuntimeError('Not running with the Werkzeug Server')
    func()


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
    server.run(host="0.0.0.0",port=5372,debug=False)
    # wait for threads to finish
    thread1.join()
    thread2.join()
    thread3.join()

    logging.info('exit')
    exit(0)
