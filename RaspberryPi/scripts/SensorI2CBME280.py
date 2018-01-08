import time
import threading
import logging
import os
import SensorI2C
import bme280
import json
"""
The sensor class for bme280 device connected over i2c.


"""

class Sensori2CBME280(SensorI2C.SensorI2C):

    def __init__(self,adress,bus):
        super().__init__(bus)
        self._adress = adress
        self._data = None
        self._exit = False
        try:
            bme280.load_calibration_params(self._bus, self._adress)
        except TypeError:
            logging.error('Failed to load  calibration data')
        pass


    def run(self):
        logging.info("entered bme280 run loop.")

        while not self._exit:
            time.sleep(1)
            self.measure()

        logging.error("sensors run left")
        return 0

    def get_json(self):
        print (".")
        logging.warning('not yet implemented')

        return json.dumps({u"humidity": self._data.humidity, \
                           u"temperature": self._data.temperature,\
                           u"pressure":self._data.pressure})


    def measure(self):
        try:
            self._data =  bme280.sample(self._bus, self._adress)
        except TypeError:
            self._data = bme280.uncompensated_readings(block=[0,0,0,0,0,0,0,0])
        return 0

    def get_string(self):
        return self._data
