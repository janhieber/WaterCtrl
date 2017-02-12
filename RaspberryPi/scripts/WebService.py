import threading
import queue
import logging
import os
import time

from flask.views import MethodView
from flask import request

"""
This is the web service class that runs a REST. The REST can be used to direct
control the mainboards functions on STM32 over SPI.
"""

""" global vars """
class RelaisView(MethodView):
    methods = ["GET","PUT","DELETE"]

    def __init__(self,app):
        self._app = app

    def get(self,id):
        logging.info(id)
        buffer = self._app.setRelais(id)
        return str('return from #%d: %s' % (id, buffer) )

    def delete(self,id):
        logging.info("del function, id= %d",id)
        self._app.clearRelais(id)
        return str('cleared realis %d' % id)

class MotorsView(MethodView):
    methods = ["GET","PUT","DELETE"]

    def __init__(self,app):
        self._app = app

    def get(self,id):
        logging.info(id)
        logging.debug('query: %s'%request.query_string)
        time = request.args.get('time')
        speed = request.args.get('speed')
        if (time != None) and (speed != None):
            logging.debug('start motor time=%s, speed=%s'%(time,speed))
            buffer = self._app.startMotor(id,int(time),int(speed))
        else:
            logging.error('bad query');
            return str('bad query'),404
        return str('return from get id=%d: %s' % (id, buffer) )

    def delete(self,id):
        logging.info("del function, id= %d",id)
        self._app.stopMotor()
        return str('stopped motor %d' % id)

class app(threading.Thread):
    SPI_SET_RELAIS = [0x16, 0x00, 0x01, 0x00, 0x00, 0x00]
    SPI_CLEAR_RELAIS = [0x16, 0x00, 0x00, 0x00, 0x00, 0x00]
    SPI_START_MOTOR = [0x10, 0x00, 0x00, 0x00, 0x00, 0x00]


    def __init__(self,server,sendQueue,recvQueue):
        threading.Thread.__init__(self)
        self.exit = False
        self._sendQueue = sendQueue
        self._recvQueue = recvQueue
        self._relais = RelaisView(self)
        self._motor = MotorsView(self)
        server.add_url_rule('/relais/<int:id>',view_func=self._relais.as_view('relais',self))
        server.add_url_rule('/motor/<int:id>',view_func=self._motor.as_view('motor',self))

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

    def waitForResponse(self):
        retry = 5
        while(True):
            try:
                buffer = self._recvQueue.get(block=True, timeout=0.9)
                break
            except queue.Empty:
                logging.warn('Queue Empty')
                retry = retry -1
                time.sleep(0.1)
                if self.exit or retry == 0:
                    buffer = []
                    break
                pass
            else:
                raise
        return buffer
