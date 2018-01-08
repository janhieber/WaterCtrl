import time
import threading
import logging
import os

"""
The sensor class for i2c device.


"""
import smbus2

class SensorI2C(threading.Thread):

    def __init__(self,bus):
        super().__init__(group=None, target=None, name='SensorI2C', args=(), kwargs=None, daemon=None)
        try:
            self._bus = smbus2.SMBus(bus);
        except (PermissionError,FileNotFoundError) as error:
            logging.error(('Failed to open i2c bus %d : %s' ) % (bus,error))
            self._bus = smbus2.SMBus()
        pass
        self._exit = True

    def run(self):
        logging.info("entered sensors run loop.")

        while True:
            logging.debug('sensorI2C.running')
            time.sleep(1000)
            self.measure()
            if self._exit:
                break

        logging.error("sensors run left")
        return 0

    def get_json(self):
        logging.Warning('not yet implemented')
        pass

    def measure(self):
        logging.debug('sensorI2C.measure')
        pass

    def get_string(self):
        pass

    def exit(self):
        self._exit = True
