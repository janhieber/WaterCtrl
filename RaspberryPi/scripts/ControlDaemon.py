#!/usr/bin/env python3
# -*- coding: utf-8 -*-

""" imports """
import threading
import queue
import time
import random
import logging
import configparser
import psycopg2
import sys


"""
    This is the main control daemon.
    It controls all high level functions
    like pouring and database stuff
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

        # query config values
        self.cycleTime = self.config.getfloat('ControlDaemon', 'cycleTime')

        # connect to db
        dbcfg = {
            'user': self.config.get('database', 'user'),
            'password': self.config.get('database', 'password'),
            'host': self.config.get('database', 'unix_socket'),
            'database': self.config.get('database', 'database')
        }

        # get a connection, if a connect cannot be made an exception will be raised here
        self.db = psycopg2.connect(**dbcfg)
        logging.info('Database connected.')


    def getAllSensors(self):
        logging.info('Read sensor values from database.')

        # open db cursor and query data
        self.dbc = self.db.cursor()
        query = ("SELECT id, channel, frequency FROM sensor")
        self.dbc.execute(query)

        # retrieve the channels from the database
        records = self.dbc.fetchall()
        self.dbc.close()

        return records


    def getMotorsForSensorChannel(self, sensorId):
        logging.info('Read motor values from database.')

        # open db cursor and query data
        self.dbc = self.db.cursor()
        query = ("SELECT id, channel, duration FROM motor WHERE sensor_id = %s" % (sensorId, ))
        self.dbc.execute(query)

        # retrieve the channels from the database
        records = self.dbc.fetchall()
        self.dbc.close()

        return records


    def run(self):
        logging.info('Starting')
        self.setup()
        nextCycle = time.time()

        while True:

            # read sensor and motor channels from database
            sensors = self.getAllSensors()
            logging.info('Received sensors from database: %s', sensors)

            for sensor in sensors:
                logging.info('Checking sensor: %s', sensor[1])
                
                
                
                # read sensor frequency
                
                # check sensor frequency against database value
                # ??? need to sleep while reading sensor value???
                
                # start motor if needed
                # ??? need to sleep while starting motor???
                
                # motors only needed if watering is required
                motors = self.getMotorsForSensorChannel(sensor[0])
                logging.info('Received motors from database: %s', motors)


            # queue some data for MessageBroker
            randomData = random.choice('abcdefghij')
            #self.sendQueue.put(randomData)
            
            # check if we should exit
            if self.exit:
                break
            
            while not self.recvQueue.empty():
                print("REC: " + self.recvQueue.get())
            
            
            # send test data
            bla = round(random.uniform(0, 1))
            if bla == 0:
                self.sendQueue.put("test bla")

            # wait for next cycle
            nextCycle = nextCycle + self.cycleTime
            sleeptime = nextCycle - time.time()
            if sleeptime >= 0.0:
                time.sleep(nextCycle - time.time())
        
        # manage exit
        self.exit_()
        logging.info('Exiting')
        return

              
    def exit_(self):
        # close db connection
        self.dbc.close()
        self.db.close()


if __name__ == '__main__':
    print('this is not meant to run alone!')
    exit(1)
