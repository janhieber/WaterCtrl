#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import psycopg2
import logging
import pygraphviz
from transitions import *
from transitions.extensions import GraphMachine as Machine

class StateMachine(object):

    def setup(self):
        
        # setup logging
        logging.basicConfig(
            level = 10,
            format = '%(asctime)s %(levelname)-8s [%(threadName)-13s] %(message)s'
        )
    
        # connect to db
        dbcfg = {
            'user': 'waterctrl_user',
            'password': 'Wfmd22nGixsP',
            'host': '/var/run/postgresql/',
            'database': 'waterctrl'
        }

        # get a connection, if a connect cannot be made an exception will be raised here
        self.db = psycopg2.connect(**dbcfg)

    def main(self):
        self.createConfigurationFromDb()
        self.createStateMachine()
        
    def createConfigurationFromDb(self):
        logging.info('Initialize configuration from the database.')
        
        self.configuration = []
        
        # open db cursor and query data
        self.dbc = self.db.cursor()
        query = ('SELECT id, channel, frequency FROM sensor')
        self.dbc.execute(query)

        # retrieve the sensors from the database
        sensors = self.dbc.fetchall()
        self.dbc.close()
        
        for sensor in sensors:
            sensor = list(sensor)
            
            # open db cursor and query data
            self.dbc = self.db.cursor()
            query = ('SELECT id, channel, duration, sensor_id FROM motor WHERE sensor_id = %s')
            self.dbc.execute(query, (sensor[1], ))

            # retrieve the motors from the database
            motors = self.dbc.fetchall()
            self.dbc.close()
            
            sensor.append(motors)
            self.configuration.append(sensor)
           
    
    #[
        #[1, 1, 100, [(1, 1, 10, 1), (2, 2, 20, 1)]], 
        #[2, 2, 200, [(3, 3, 30, 2), (4, 4, 40, 2)]], 
        #[3, 3, 300, [(5, 5, 50, 3), (6, 6, 60, 3)]]]
    def createStateMachine(self):
        logging.info('STATEMACHINE: Creating StateMaching...')
        
        states = []
        transitions = []
        
        states.append(State('SLEEP'))
        
        for sensorIndex, sensor in enumerate(self.configuration):
            # Add sensor states
            states.append(State('READ_SENSOR_'+str(sensor[1])))
            states.append(State('RECEIVE_SENSOR_'+str(sensor[1])))
            states.append(State('CHECK_VALUE_'+str(sensor[1])))
            
            # Add sensor transitions
            if(sensorIndex == 0):
                transitions.append(['read_sensor', 'SLEEP', 'READ_SENSOR_'+str(sensor[1])])

            transitions.append(['receive_sensor', 'READ_SENSOR_'+str(sensor[1]), 'RECEIVE_SENSOR_'+str(sensor[1])])
            transitions.append(['check_sensor', 'RECEIVE_SENSOR_'+str(sensor[1]), 'CHECK_VALUE_'+str(sensor[1])])
            
            if(sensorIndex == len(self.configuration)-1):
                transitions.append(['value_checked', 'CHECK_VALUE_'+str(sensor[1]), 'SLEEP'])
            else:
                transitions.append(['value_checked', 'CHECK_VALUE_'+str(sensor[1]), 'READ_SENSOR_'+str(sensor[1]+1)])
        
            motors = sensor[3]
            for motorIndex, motor in enumerate(motors):
                # Add motor states
                states.append(State('START_MOTOR_'+str(motor[1])))
                states.append(State('RECEIVE_MOTOR_'+str(motor[1])))
            
                # Add motor transistions
                if(motorIndex == 0):
                    transitions.append(['start_motor', 'CHECK_VALUE_'+str(sensor[1]), 'START_MOTOR_'+str(motor[1])])
 
                transitions.append(['receive_motor', 'START_MOTOR_'+str(motor[1]), 'RECEIVE_MOTOR_'+str(motor[1])])
                
                if(motorIndex != len(motors)-1):
                    transitions.append(['process_receive_motor', 'RECEIVE_MOTOR_'+str(motor[1]), 'START_MOTOR_'+str(motor[1]+1)])
                else:
                    if(sensorIndex == len(self.configuration)-1):
                        transitions.append(['process_receive_motor', 'RECEIVE_MOTOR_'+str(motor[1]), 'SLEEP'])
                    else:
                        transitions.append(['process_receive_motor', 'RECEIVE_MOTOR_'+str(motor[1]), 'READ_SENSOR_'+str(sensor[1]+1)])
            
        # Initialize the state machine
        self.machine = Machine(model=self, states=states, auto_transitions=False, initial='SLEEP')

        for transition in transitions:
            print(transition)
            self.machine.add_transition(transition[0], transition[1], transition[2])
  
        
  
        self.machine.graph.draw('state_machine_diagram.png', prog='dot')
        logging.info('STATEMACHINE: Finished creating StateMachine!')    


if __name__ == '__main__':
    stateMachine = StateMachine()
    stateMachine.setup()
    stateMachine.main()
    exit(0)