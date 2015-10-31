#!/usr/bin/env python3
# -*- coding: utf-8 -*-

""" imports """
import threading
import queue
import time
import random
import logging
import mysql.connector
import configparser



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
        self.config = configparser.RawConfigParser()
        self.config.read('WaterCtrl.conf')

    def setup(self):
        # connect to db
        self.db = mysql.connector.connect(
            user = self.config.get('database', 'user'),
            password = self.config.get('database', 'password'),
            host = self.config.get('database', 'host'),
            unix_socket = self.config.get('database', 'unix_socket'),
            database = self.config.get('database', 'database')
         )
                              
    def run(self):
        logging.info('Starting')
        self.setup()
        while True:
            # open db cursor and query data
            self.dbc = self.db.cursor()
            query = ("SELECT * FROM bla")
            self.dbc.execute(query)
            self.dbc.close()
            
            # queue some data for MessageBroker
            randomData = random.choice('abcdefghij')
            self.queue.put(randomData)
            
            # check if we should exit
            if self.exit:
                break
            
            # sleep random time 0.1 - 2.0 sec
            time.sleep(random.uniform(0.1, 2.0))
        
        # manage exit
        self.exit()
        logging.info('Exiting')
        return

              
    def exit(self):
        # close db connection
        self.dbc.close()
        self.db.close()


if __name__ == '__main__':
    print('this is not meant to run alone!')
    exit(1)
