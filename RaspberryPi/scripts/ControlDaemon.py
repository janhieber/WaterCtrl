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
        query = ('SELECT id, channel, frequency FROM sensor')
        self.dbc.execute(query)

        # retrieve the channels from the database
        records = self.dbc.fetchall()
        self.dbc.close()

        return records


    def getMotorsForSensorId(self, sensorId):
        logging.info('Read motor values from database.')

        # open db cursor and query data
        self.dbc = self.db.cursor()
        query = ('SELECT id, channel, duration, sensor_id FROM motor WHERE sensor_id = %s')
        self.dbc.execute(query, (sensorId, ))

        # retrieve the channels from the database
        records = self.dbc.fetchall()
        self.dbc.close()

        return records
        
    
    def getPlantsForMotorId(self, motorId):
        logging.info('Read plants from database.')

        # open db cursor and query data
        self.dbc = self.db.cursor()
        query = ('SELECT id, name, description, watering_interval, motor_id FROM plant WHERE motor_id = %s')
        self.dbc.execute(query, (motorId, ))

        # retrieve the channels from the database
        records = self.dbc.fetchall()
        self.dbc.close()

        return records
        
    
    # insert log entry to database
    # log states are:
    #   0: error
    #   1: warning
    #   2: success
    def insertLogEntry(self, logMessage, logState):
        logging.info('Insert log entry to database. Status: %s, Message: %s', logState, logMessage)
        
        # open db cursor and insert data
        self.dbc = self.db.cursor()
        query = ('INSERT INTO log (log_entry, log_state, log_date) VALUES (%s, %s, %s)')
        self.dbc.execute(query, (logMessage, logState, time.strftime('%Y-%m-%d %H:%M:%S')))
        self.db.commit()
        self.dbc.close()
        
    # insert watering entry to database
    # watering states are:
    #   0: nok
    #   1: ok
    def insertWateringEntry(self, wateringDuration, wateringState, plantId):
        logging.info('Insert watering entry to database. Duration: %s, State: %s, Plant Id: %s', wateringDuration, wateringState, plantId)
        
        # open db cursor and insert data
        self.dbc = self.db.cursor()
        query = ('INSERT INTO watering (watering_date, watering_duration, watering_state, plant_id) VALUES (%s, %s, %s, %s)')
        self.dbc.execute(query, (time.strftime('%Y-%m-%d %H:%M:%S'), wateringDuration, wateringState, plantId))
        self.db.commit()
        self.dbc.close()
    

    def run(self):
        logging.info('Starting')
        self.setup()
        nextCycle = time.time()

        # sensor[0] = id
        # sensor[1] = channel
        # sensor[2] = frequency
        
        # motor[0] = id
        # motor[1] = channel
        # motor[2] = duration
        # motor[3] = sensor_id
        
        # plant[0] = id
        # plant[1] = name
        # plant[2] = description
        # plant[3] = watering_interval
        # plant[4] = motor_id

        while True:

            # read sensor and motor channels from database
            sensors = self.getAllSensors()
            logging.info('Received sensors from database: %s', sensors)

            for sensor in sensors:
                logging.info('Checking sensor: %s', sensor[1])
                
            
                # read sensor frequency
                
                # check sensor frequency against database value
                # ??? need to sleep while reading sensor value???
                
                sensorvalue = 200
                
                # if measured value is smaller than value from database, watering is needed
                if sensorvalue <= sensor[2]:
                    # motors only needed if watering is required
                    motors = self.getMotorsForSensorId(sensor[0])
                    logging.info('Received motors for sensor %s from database: %s', sensor[1], motors)
                    
                    # start motor if needed
                    # ??? need to sleep while starting motor???
                    for motor in motors:
                        logging.info('Start watering for motor: %s', motor[1])
                        #self.insertLogEntry('stubbed text', 2)
                        
                        # read plants for logging entries
                        plants = self.getPlantsForMotorId(motor[0])
                        logging.info('Received plants for motor %s from database: %s', motor[1], plants)
                        
                        
                        
                        for plant in plants:
                            logging.info('Checking plant: %s', plant[1])
                            # check if watering was ok
                            # query sensor values from arm for check
                            #self.insertWateringEntry(motor[2], 1, plant[0])
                        
                            
                    
                


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
