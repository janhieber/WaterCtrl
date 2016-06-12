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
import datetime
from transitions import *



"""
    This is the main control daemon.
    It controls all high level functions
    like pouring and database stuff

    0x01 - Error message as ASCII string
    0x02 - Info message as ASCII string
    0x03 - Debug message as ASCII string
    0x10 - Motor control command
        2. byte: motor number
        3. byte: ramp up time
        4. byte: time to run
        5. byte: ramp down time
        6. byte: max speed: 0-100% (of PWM)
    0x11 - Motor control response
        2. byte: motor number
        3. byte: 0xab = done | 0xac = error
    0x12 - Moisture measure request
        2. byte: sensor number
    0x13 - Moisture measure value
        2. byte: sensor number
        3. byte: moisture value scaled to 8bit unsigned (kHz)
"""
class app(threading.Thread):
    
    SPI_READ_SENSOR_1 = [0x12, 0x01, 0x00, 0x00, 0x00, 0x00]
    SPI_READ_SENSOR_2 = [0x12, 0x02, 0x00, 0x00, 0x00, 0x00]
    SPI_READ_SENSOR_3 = [0x12, 0x03, 0x00, 0x00, 0x00, 0x00]
    SPI_READ_SENSOR_4 = [0x12, 0x04, 0x00, 0x00, 0x00, 0x00]
    SPI_READ_SENSOR_5 = [0x12, 0x05, 0x00, 0x00, 0x00, 0x00]
    
    SPI_START_MOTOR_1 = [0x10, 0x01, 0x1E, 0x64, 0x00, 0x00]
    SPI_START_MOTOR_2 = [0x10, 0x02, 0x1E, 0x64, 0x00, 0x00]
    SPI_START_MOTOR_3 = [0x10, 0x03, 0x1E, 0x64, 0x00, 0x00]
    SPI_START_MOTOR_4 = [0x10, 0x04, 0x1E, 0x64, 0x00, 0x00]
    SPI_START_MOTOR_5 = [0x10, 0x05, 0x1E, 0x64, 0x00, 0x00]
    
    SPI_RECEIVE_SENSOR_VALUE = 19
    SPI_RECEIVE_MOTOR_RESPONSE = 17

    def __init__(self, sendQueue, recvQueue):
        threading.Thread.__init__(self)
        self.sendQueue = sendQueue
        self.recvQueue = recvQueue
        self.exit = False
        self.config = configparser.RawConfigParser()
        self.config.read('WaterCtrl.conf')
        
        
    def setup(self):
        
        # query active and inactive times
        self.wateringActiveTime = datetime.datetime.strptime(self.config.get('ControlDaemon', 'wateringActiveTime'), "%H:%M").time()
        self.wateringInactiveTime = datetime.datetime.strptime(self.config.get('ControlDaemon', 'wateringInactiveTime'), "%H:%M").time()
        logging.info('Configured Active Time: %s', self.wateringActiveTime)
        logging.info('Configured Inactive Time: %s', self.wateringInactiveTime)

        # query config values
        self.cycleTime = self.config.getfloat('general', 'cycleTime')
        self.wateringCycleTime = self.config.getfloat('ControlDaemon', 'wateringCycleTime')

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


    def createConfigurationFromDb(self):
        logging.info('Initialize configuration from the database.')
        
        self.configuration = []
        
        # open db cursor and query data
        self.dbc = self.db.cursor()
        query = ('SELECT sensor_channel, frequency FROM sensor ORDER BY sensor_channel')
        self.dbc.execute(query)

        # retrieve the sensors from the database
        sensors = self.dbc.fetchall()
        self.dbc.close()
        
        for sensor in sensors:
            sensor = list(sensor)
            
            # open db cursor and query data
            self.dbc = self.db.cursor()
            query = ('SELECT motor_channel, duration, sensor_channel FROM motor WHERE sensor_channel = %s ORDER BY motor_channel')
            self.dbc.execute(query, (sensor[0], ))

            # retrieve the motors from the database
            motors = self.dbc.fetchall()
            self.dbc.close()
            
            sensor.append(motors)
            self.configuration.append(sensor)
           

    def createStateMachine(self):
        logging.info('STATEMACHINE: Creating StateMaching...')
        
        states = []
        transitions = []
        
        states.append(State('SLEEP'))
        
        for sensorIndex, sensor in enumerate(self.configuration):
            # Add sensor states
            states.append(State('READ_SENSOR_'+str(sensor[0])))
            states.append(State('RECEIVE_SENSOR_'+str(sensor[0])))
            states.append(State('CHECK_VALUE_'+str(sensor[0])))
            
            # Add sensor transitions
            if(sensorIndex == 0):
                transitions.append(['read_sensor', 'SLEEP', 'READ_SENSOR_'+str(sensor[0])])

            transitions.append(['receive_sensor', 'READ_SENSOR_'+str(sensor[0]), 'RECEIVE_SENSOR_'+str(sensor[0])])
            transitions.append(['check_sensor', 'RECEIVE_SENSOR_'+str(sensor[0]), 'CHECK_VALUE_'+str(sensor[0])])
            
            if(sensorIndex == len(self.configuration)-1):
                transitions.append(['value_checked', 'CHECK_VALUE_'+str(sensor[0]), 'SLEEP'])
            else:
                transitions.append(['value_checked', 'CHECK_VALUE_'+str(sensor[0]), 'READ_SENSOR_'+str(sensor[0]+1)])
        
            motors = sensor[2]
            for motorIndex, motor in enumerate(motors):
                # Add motor states
                states.append(State('START_MOTOR_'+str(motor[0])))
                states.append(State('RECEIVE_MOTOR_'+str(motor[0])))
            
                # Add motor transistions
                if(motorIndex == 0):
                    transitions.append(['start_motor', 'CHECK_VALUE_'+str(sensor[0]), 'START_MOTOR_'+str(motor[0])])
 
                transitions.append(['receive_motor', 'START_MOTOR_'+str(motor[0]), 'RECEIVE_MOTOR_'+str(motor[0])])
                
                if(motorIndex != len(motors)-1):
                    transitions.append(['process_receive_motor', 'RECEIVE_MOTOR_'+str(motor[0]), 'START_MOTOR_'+str(motor[0]+1)])
                else:
                    if(sensorIndex == len(self.configuration)-1):
                        transitions.append(['process_receive_motor', 'RECEIVE_MOTOR_'+str(motor[0]), 'SLEEP'])
                    else:
                        transitions.append(['process_receive_motor', 'RECEIVE_MOTOR_'+str(motor[0]), 'READ_SENSOR_'+str(sensor[0]+1)])

        # Initialize the state machine
        self.machine = Machine(model=self, states=states, auto_transitions=False, initial='SLEEP')

        for transition in transitions:
            self.machine.add_transition(transition[0], transition[1], transition[2])
  
        logging.info('STATEMACHINE: Finished creating StateMachine!')
  
          
    def getSensorFrequencyForSensorChannel(self, sensorChannel):
        # open db cursor and query data
        self.dbc = self.db.cursor()
        query = ('SELECT frequency FROM sensor WHERE sensor_channel = %s')
        self.dbc.execute(query, (sensorChannel, ))

        # retrieve the channels from the database
        frequency = self.dbc.fetchone()
        self.dbc.close()

        return frequency[0]
      
     
    def getMotorDurationForMotorChannel(self, motorChannel):
        # open db cursor and query data
        self.dbc = self.db.cursor()
        query = ('SELECT duration FROM motor WHERE motor_channel = %s') 
        self.dbc.execute(query, (motorChannel, ))

        # retrieve the channels from the database
        duration = self.dbc.fetchone()
        self.dbc.close()

        return duration[0]
        

    def getPlantIdsForMotorChannel(self, motorChannel):
        logging.info('Read plants from database.')

        # open db cursor and query data
        self.dbc = self.db.cursor()
        query = ('SELECT id FROM plant WHERE motor_channel = %s')
        self.dbc.execute(query, (motorChannel, ))

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
        
    # insert sensor response to database
    def insertSensorResponseEntry(self, sensorChannel, sensorValue):
        logging.info('Insert sensor response to database. Channel: %s, Frequency: %s', sensorChannel, sensorValue)

        # open db cursor and insert data
        self.dbc = self.db.cursor()
        query = ('INSERT INTO sensor_response (sensor_channel, frequency, measure_date) VALUES (%s, %s, %s)')
        self.dbc.execute(query, (sensorChannel, sensorValue, time.strftime('%Y-%m-%d %H:%M:%S')))
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


    def on_enter_READ_SENSOR_1(self):
        logging.info('Sending message: %s', self.SPI_READ_SENSOR_1)
        self.sendQueue.put(self.SPI_READ_SENSOR_1)
        
    def on_enter_RECEIVE_SENSOR_1(self, value1, value2):
        sixteenBitValue = 256*value1 + value2;
        logging.info('Sensor 1 value: %s', sixteenBitValue)
        self.insertSensorResponseEntry(1, sixteenBitValue)
        self.lastSensorValue = sixteenBitValue
        self.check_sensor()
        
    def on_enter_CHECK_VALUE_1(self):
        frequency = self.getSensorFrequencyForSensorChannel(1)
        logging.info('Value from sensor 1 is: %s', self.lastSensorValue)
        logging.info('Frequency from db is: %s', frequency)
        
        if(frequency < self.lastSensorValue):
            self.start_motor()
        else:
            self.value_checked()
        
    def on_enter_READ_SENSOR_2(self):
        logging.info('Sending message: %s', self.SPI_READ_SENSOR_2)
        self.sendQueue.put(self.SPI_READ_SENSOR_2)
        
    def on_enter_RECEIVE_SENSOR_2(self, value1, value2):
        sixteenBitValue = 256*value1 + value2;
        logging.info('Sensor 2 value: %s', sixteenBitValue)
        self.insertSensorResponseEntry(2, sixteenBitValue)
        self.lastSensorValue = sixteenBitValue
        self.check_sensor()
        
    def on_enter_CHECK_VALUE_2(self):
        frequency = self.getSensorFrequencyForSensorChannel(1)
        logging.info('Value from sensor 2 is: %s', self.lastSensorValue)
        logging.info('Frequency from db is: %s', frequency)
        
        if(frequency < self.lastSensorValue):
            self.start_motor()
        else:
            self.value_checked()
    
    def on_enter_READ_SENSOR_3(self):
        logging.info('Sending message: %s', self.SPI_READ_SENSOR_3)
        self.sendQueue.put(self.SPI_READ_SENSOR_3)
    
    def on_enter_RECEIVE_SENSOR_3(self, value1, value2):
        sixteenBitValue = 256*value1 + value2;
        logging.info('Sensor 3 value: %s', sixteenBitValue)
        self.insertSensorResponseEntry(3, sixteenBitValue)
        self.lastSensorValue = sixteenBitValue
        self.check_sensor()
        
    def on_enter_CHECK_VALUE_3(self):
        frequency = self.getSensorFrequencyForSensorChannel(1)
        logging.info('Value from sensor 3 is: %s', self.lastSensorValue)
        logging.info('Frequency from db is: %s', frequency)
        
        if(frequency < self.lastSensorValue):
            self.start_motor()
        else:
            self.value_checked()
    
    def on_enter_READ_SENSOR_4(self):
        logging.info('Sending message: %s', self.SPI_READ_SENSOR_4)
        self.sendQueue.put(self.SPI_READ_SENSOR_4)
        
    def on_enter_RECEIVE_SENSOR_4(self, value1, value2):
        sixteenBitValue = 256*value1 + value2;
        logging.info('Sensor 4 value: %s', sixteenBitValue)
        self.insertSensorResponseEntry(4, sixteenBitValue)
        self.lastSensorValue = sixteenBitValue
        self.check_sensor()
        
    def on_enter_CHECK_VALUE_4(self):
        frequency = self.getSensorFrequencyForSensorChannel(1)
        logging.info('Value from sensor 4 is: %s', self.lastSensorValue)
        logging.info('Frequency from db is: %s', frequency)
        
        if(frequency < self.lastSensorValue):
            self.start_motor()
        else:
            self.value_checked()
    
    def on_enter_READ_SENSOR_5(self):
        logging.info('Sending message: %s', self.SPI_READ_SENSOR_5)
        self.sendQueue.put(self.SPI_READ_SENSOR_5)
        
    def on_enter_RECEIVE_SENSOR_5(self, value1, value2):
        sixteenBitValue = 256*value1 + value2;
        logging.info('Sensor 5 value: %s', sixteenBitValue)
        self.insertSensorResponseEntry(5, sixteenBitValue)
        self.lastSensorValue = sixteenBitValue
        self.check_sensor()
        
    def on_enter_CHECK_VALUE_5(self):
        frequency = self.getSensorFrequencyForSensorChannel(1)
        logging.info('Value from sensor 5 is: %s', self.lastSensorValue)
        logging.info('Frequency from db is: %s', frequency)
        
        if(frequency < self.lastSensorValue):
            self.start_motor()
        else:
            self.value_checked()
            
    def on_enter_START_MOTOR_1(self):
        self.lastDuration = self.getMotorDurationForMotorChannel(1)
        self.SPI_START_MOTOR_1[2] = self.lastDuration
        logging.info('Sending message: %s', self.SPI_START_MOTOR_1)
        self.sendQueue.put(self.SPI_START_MOTOR_1)
        time.sleep(self.lastDuration+1)
        
    #0xab = 171 = done/finish | 0xac = 172 = error
    def on_enter_RECEIVE_MOTOR_1(self, response):
        plantIds = self.getPlantIdsForMotorChannel(1)
        if(response == 171):
            for plantId in plantIds:
                self.insertWateringEntry(self.lastDuration, 1, plantId)
        else:
            for plantId in plantIds:
                self.insertWateringEntry(self.lastDuration, 0, plantId)
        self.process_receive_motor()
        
        
    def on_enter_START_MOTOR_2(self):
        self.lastDuration = self.getMotorDurationForMotorChannel(2)
        self.SPI_START_MOTOR_2[2] = self.lastDuration
        logging.info('Sending message: %s', self.SPI_START_MOTOR_2)
        self.sendQueue.put(self.SPI_START_MOTOR_2)
        time.sleep(self.lastDuration+1)
        
    #0xab = 171 = done/finish | 0xac = 172 = error
    def on_enter_RECEIVE_MOTOR_2(self, response):
        plantIds = self.getPlantIdsForMotorChannel(2)
        if(response == 171):
            for plantId in plantIds:
                self.insertWateringEntry(self.lastDuration, 1, plantId)
        else:
            for plantId in plantIds:
                self.insertWateringEntry(self.lastDuration, 0, plantId)
        self.process_receive_motor()
    
    def on_enter_START_MOTOR_3(self):
        self.lastDuration = self.getMotorDurationForMotorChannel(3)
        self.SPI_START_MOTOR_3[2] = self.lastDuration
        logging.info('Sending message: %s', self.SPI_START_MOTOR_3)
        self.sendQueue.put(self.SPI_START_MOTOR_3)
        time.sleep(self.lastDuration+1)
        
    #0xab = 171 = done/finish | 0xac = 172 = error
    def on_enter_RECEIVE_MOTOR_3(self, response):
        plantIds = self.getPlantIdsForMotorChannel(3)
        if(response == 171):
            for plantId in plantIds:
                self.insertWateringEntry(self.lastDuration, 1, plantId)
        else:
            for plantId in plantIds:
                self.insertWateringEntry(self.lastDuration, 0, plantId)
        self.process_receive_motor()
        
    def on_enter_START_MOTOR_4(self):
        self.lastDuration = self.getMotorDurationForMotorChannel(4)
        self.SPI_START_MOTOR_4[2] = self.lastDuration
        logging.info('Sending message: %s', self.SPI_START_MOTOR_4)
        self.sendQueue.put(self.SPI_START_MOTOR_4)
        time.sleep(self.lastDuration+1)
        
    #0xab = 171 = done/finish | 0xac = 172 = error
    def on_enter_RECEIVE_MOTOR_4(self, response):
        plantIds = self.getPlantIdsForMotorChannel(4)
        if(response == 171):
            for plantId in plantIds:
                self.insertWateringEntry(self.lastDuration, 1, plantId)
        else:
            for plantId in plantIds:
                self.insertWateringEntry(self.lastDuration, 0, plantId)
        self.process_receive_motor()
        
    def on_enter_START_MOTOR_5(self):
        self.lastDuration = self.getMotorDurationForMotorChannel(5)
        self.SPI_START_MOTOR_5[2] = self.lastDuration
        logging.info('Sending message: %s', self.SPI_START_MOTOR_5)
        self.sendQueue.put(self.SPI_START_MOTOR_5)
        time.sleep(self.lastDuration+1)
        
    #0xab = 171 = done/finish | 0xac = 172 = error
    def on_enter_RECEIVE_MOTOR_5(self, response):
        plantIds = self.getPlantIdsForMotorChannel(5)
        if(response == 171):
            for plantId in plantIds:
                self.insertWateringEntry(self.lastDuration, 1, plantId)
        else:
            for plantId in plantIds:
                self.insertWateringEntry(self.lastDuration, 0, plantId)
        self.process_receive_motor()


    def run(self):
        logging.info('Starting')
        self.setup()
        nextCycle = time.time()
        nextWateringCycle = time.time()
        showInactiveLoggingOutput = True

        # sensor[0] = sensor_channel
        # sensor[1] = frequency

        # motor[0] = motor_channel
        # motor[1] = duration
        # motor[2] = sensor_channel

        # plant[0] = id
        # plant[1] = name
        # plant[2] = description
        # plant[3] = watering_interval
        # plant[4] = motor_channel
        
        self.createConfigurationFromDb()
        self.createStateMachine()
        
        
        while True:
            
            # check if watering cycle time is inactive
            timeNow = datetime.datetime.now().time()

            if timeNow > self.wateringInactiveTime or timeNow < self.wateringActiveTime and self.is_SLEEP():
                if(showInactiveLoggingOutput):
                    logging.info("Watering cycle is inactive. Starts at: %s", self.wateringActiveTime)
                    showInactiveLoggingOutput = False

            else:
                # watering implementation has different cycle time
                # wait for next cycle
                if nextWateringCycle - time.time() <= 0.0:
                    # set time for next watering cycle
                    nextWateringCycle = nextWateringCycle + self.wateringCycleTime
                    
                    logging.info('NEXT CYLCE: %s', nextWateringCycle - time.time())
                    
                    self.createConfigurationFromDb()
                    self.createStateMachine()
                    self.read_sensor()

                # process receive queue
                if (not self.recvQueue.empty()):
                    recvbuf = self.recvQueue.get()
            
                    if len(recvbuf) == 6:
                        if recvbuf[0] == self.SPI_RECEIVE_SENSOR_VALUE:
                            self.receive_sensor(recvbuf[2], recvbuf[3])
                        elif recvbuf[0] == self.SPI_RECEIVE_MOTOR_RESPONSE:
                            self.receive_motor(recvbuf[1])
                
                # wait for next cycle
                nextCycle = nextCycle + self.cycleTime
                sleeptime = nextCycle - time.time()
                
                if sleeptime >= 0.0:
                    time.sleep(sleeptime)
                    
                showInactiveLoggingOutput = True
                
            # check if we should exit
            if self.exit:
                break
        
        # manage exit
        self.exit_()
        logging.info('Exiting')
        return

              
    def exit_(self):
        # close db connection
        self.db.close()


if __name__ == '__main__':
    print('this is not meant to run alone!')
    exit(1)