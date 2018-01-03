import threading
import queue
import logging
import os
import time

from flask.views import MethodView
from flask import request
from flask_restplus import Api, Resource, fields, reqparse

"""
This is the web service class that runs a REST. The REST can be used to direct
control the mainboards functions on STM32 over SPI.
"""

""" global vars """
gApi = Api(version='0.1',title='REST 2 SPI',description='REST to SPI interface')
#gServerApplicaton = self._app

class WebServiceResource(Resource):

    def __init__(self,api=None,*args,**kwargs):
        super().__init__(api,args,kwargs)
        logging.info('init got args: %s , kwargs: %s '%(args,kwargs))
        self._app = kwargs['app']
        logging.info('resource created')


@gApi.doc(repsonse={'404':'bad channel number'})
class RelaisView(WebServiceResource):
    #methods = ["GET","PUT","DELETE"]
    """
    Relais class.

    ** GET ** to set the relais
    ** DELETE ** to clear the relais
    """

    def get(self,id):
        logging.info(id)
        buffer = self._app.setRelais(id)
        return str('return from #%d: %s' % (id, buffer) )

    def delete(self,id):
        logging.info("del function, id= %d",id)
        self._app.clearRelais(id)
        return str('cleared realis %d' % id)

@gApi.doc(repsonse={'404':'bad channel number'})
class MotorsView(WebServiceResource):
    #methods = ["GET","PUT","DELETE"]
    """
    Sensor class.

    ** GET ** to start the motor state
    ** DELETE ** to stop the motor
    """

    def get(self,id):
        logging.info(id)
        logging.debug('query: %s'%request.query_string)
        time = request.args.get('time')
        speed = request.args.get('speed')
        if (time != None) and (speed != None):
            logging.debug('start motor time=%s, speed=%s'%(time,speed))
            buffer = self._app.startMotor(id,int(time),int(speed))
        else:
            logging.error('bad query')
            return str('bad query'),404
        return str('return from get id=%d: %s' % (id, buffer) )

    def delete(self,id):
        logging.info("del function, id= %d",id)
        self._app.stopMotor()
        return str('stopped motor %d' % id)


@gApi.doc(response={'404':'bad channel number'})
class SensorsView(Resource):
    """
    Sensor class.

    ** GET ** to read the sensor state
    ** PUT ** change the sensor type
    """
    #methods = ["GET","PUT"]
#    def __init__(self,api=None,*args,**kwargs):#
#        super().__init__(api,args,kwargs)
#        logging.info('init got args: %s , kwargs: %s '%(args,kwargs))
#        self._app = kwargs['app']
#        logging.info('sensor resource created')

    def get(self,id):
        """
        Read a sensor.
        ** GET ** to read the sensors value.
        :param int id: number of channel
        returns: str
        """
        logging.debug('get sensor: %d'%id)
        buffer = self._app.getSensor(id)
        #ServerApplicaton.getSensor(id)
        return str('return from get id=%d: %s' % (id, buffer) )

    def put(self,id,type=None):
        """
        Change a sensor type.

        ** PUT ** to change the sensors type. Give type in url or as query_string.

        :param int id: number of channel
        :param int type: enum value {moisture,dht22_hum,dht22_temp,analog_pa2,analog_pa3}
        :returns: str
        """
        if (type != None):
            logging.info('set type: %s on id: %d'%(type,id))
            self._app.setSensorType(id, int(type))
        else:
            type = request.args.get('type')
            if (type == None):
                logging.error('bad query')
                return str('bad query'),404
        return str('ok'),200

class app(threading.Thread):
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
