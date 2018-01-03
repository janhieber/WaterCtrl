import threading
import queue
import logging
import os
import time

from flask import request
from flask_restplus import Api, Resource, fields, reqparse

"""
This flash restplus API describes the sensor resource.
"""

gApi = Api(version='0.1',title='REST 2 SPI',description='REST sensor')

class APISensor:
    def __init__(self,server=None,hwsensors=[]):
        logging.warning("__init__ of API called")
        #self._api = Api(version='0.1',title='REST 2 SPI',description='REST sensor')
        global gApi
        gApi.add_resource(SensorsView,'/sensor/<int:id>',resource_class_kwargs={'app':self,},methods=['GET'])
        gApi.add_resource(SensorsView,'/sensor/<int:id>/<int:type>',resource_class_kwargs={'app':self,},methods=['PUT'])
        #gApi.add_resource(RelaisView,'/relais/<int:id>',resource_class_kwargs={'app':self,},methods=['GET','DELETE'])
        #gApi.add_resource(MotorsView,'/motor/<int:id>',resource_class_kwargs={'app':self,},methods=['GET','DELETE'])
        gApi.init_app(server)
        logging.info("__init__ of API done")

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
class SensorsView(WebServiceResource):
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
