import threading
import queue
import logging
import os
import time

class Sensor(threading.Thread):
    SPI_SET_RELAIS = [0x16, 0x00, 0x01, 0x00, 0x00, 0x00]
    SPI_CLEAR_RELAIS = [0x16, 0x00, 0x00, 0x00, 0x00, 0x00]
    SPI_START_MOTOR = [0x10, 0x00, 0x00, 0x00, 0x00, 0x00]
    SPI_SET_SENS_TYPE = [20, 0, 0, 0, 0, 0]
    SPI_GET_SENS = [18, 0, 0, 0, 0, 0]

    def __init__(self,server,sendQueue,recvQueue):
        global gApi
        threading.Thread.__init__(self)
        self.exit = False
        self._sendQueue = sendQueue
        self._recvQueue = recvQueue

        self._api = gApi
        self._api.add_resource(SensorsView,'/sensor/<int:id>',resource_class_kwargs={'app':self,},methods=['GET'])
        self._api.add_resource(SensorsView,'/sensor/<int:id>/<int:type>',resource_class_kwargs={'app':self,},methods=['PUT'])
        self._api.add_resource(RelaisView,'/relais/<int:id>',resource_class_kwargs={'app':self,},methods=['GET','DELETE'])
        self._api.add_resource(MotorsView,'/motor/<int:id>',resource_class_kwargs={'app':self,},methods=['GET','DELETE'])
        self._api.init_app(server)

    def run(self):
        logging.info("starting the web server on port 5372")

        while True:
            time.sleep(100)

            if self.exit:
                break

        logging.error("WebService run left")
        return 0

    def setRelais(self,id):
        self.SPI_SET_RELAIS[1] = id
        logging.debug('setRelais #%d' % id)
        self._sendQueue.put(self.SPI_SET_RELAIS)

        buffer = self.waitForResponse()

        logging.debug('setRelais - buffer %s' %buffer)
        return buffer

    def clearRelais(self,id):
        self.SPI_CLEAR_RELAIS[1] = id
        logging.debug('clearRelais #%d' % id)
        self._sendQueue.put(self.SPI_CLEAR_RELAIS)
        return ('clearRelais %d : %s ' % (id,self.waitForResponse()))

    def startMotor(self,id,time,speed):
        self.SPI_START_MOTOR[1] = id
        self.SPI_START_MOTOR[2] = time
        self.SPI_START_MOTOR[3] = speed
        logging.debug('startMotor #%d, time=%d, speed=%d' % (id,time,speed))
        self._sendQueue.put(self.SPI_START_MOTOR)
        return ('startMotor %d : %s ' % (id,self.waitForResponse()))

    def stopMotor(self):
        self.SPI_START_MOTOR[1] = 0
        self.SPI_START_MOTOR[2] = 0
        self.SPI_START_MOTOR[3] = 0
        logging.debug('StopMotor')
        self._sendQueue.put(self.SPI_START_MOTOR)
        return ('StopMotor: %s ' % self.waitForResponse())

    def getSensor(self,id):
        self.SPI_GET_SENS[1] = id
        logging.debug('getSensor')
        self._sendQueue.put(self.SPI_GET_SENS)
        return ('ret getSensor: %s ' % self.waitForResponse())

    def setSensorType(self,id,type):
        self.SPI_SET_SENS_TYPE[1] = id
        self.SPI_SET_SENS_TYPE[3] = type
        self._sendQueue.put(self.SPI_SET_SENS_TYPE, block=True, timeout=None)
        return ('ret setSensorType: %s ' % self.waitForResponse())

    def waitForResponse(self):
        retry = 1
        while(True):
            try:
                buffer = self._recvQueue.get(block=True, timeout=1)
                break
            except queue.Empty:
                logging.warn('Queue Empty, retry = %d' % retry)
                time.sleep(0.1)
                if self.exit or retry == 0:
                    buffer = []
                    break
                else:
                    retry = retry -1
                pass
            else:
                raise
        return buffer
