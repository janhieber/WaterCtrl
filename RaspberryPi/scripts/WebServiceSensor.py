#!/usr/bin/env python3
# -*- coding: utf-8 -*-

""" imports """
import threading
import logging
import signal
import sys
import configparser
import os

# add more supported sensors
#import SensorSPISTM32
import SensorI2CBME280

# add more API's
import APISensor
#import APIMotor
#import APIRelais

from flask import Flask, request

""" global vars """
config = None
_hwSensors = []

server = Flask(__name__)

"""
    Setup everything
"""
def setup():
    global _hwSensors
    # read config
    print ('setup() entered')
    os.path.dirname(os.path.abspath(__file__))
    config = configparser.ConfigParser()
    config.read('Sensor.conf')
    config.sections()

    # setup logging
    logging.basicConfig(
        filename = config.get('general', 'logfile'),
        level = config.getint('general', 'loglevel'),
        format = '%(asctime)s %(levelname)-8s [%(threadName)-13s] %(message)s'
    )

    # setup connected sensors
    if config.has_section("BME280"):
        _hwSensors.append(SensorI2CBME280.Sensori2CBME280(
            adress = int(config.get('BME280','adress',raw=True),16),
            bus = config.getint('BME280', 'port'))
        )

    apiSensor = APISensor.APISensor(server,_hwSensors)

    # setup signal traps
    signal.signal(signal.SIGINT, signal_handler)
    signal.signal(signal.SIGUSR1, signal_handler)
    signal.signal(signal.SIGUSR2, signal_handler)
    logging.critical('everything set up')

"""
    Main function
"""
@server.before_first_request
def main():
    logging.info('main function')
    for sensor in _hwSensors:
        sensor.start()

"""
    Exit handler.
    Tries to shutdown the app graceful.
"""
def gracefulExit():
    logging.critical('exit graceful')
    for sensor in _hwSensors:
        sensor.join()
        sensor.exit = True

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
    print('mein main')
    setup()
    logging.debug('running main')
    server.run(host="0.0.0.0",port=5373,debug=True,)
    logging.info('exit')
    exit(0)
