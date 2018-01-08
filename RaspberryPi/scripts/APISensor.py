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

class APISensor():
    def __init__(self,server=None,hwsensors=[]):
        logging.warning("__init__ of API called")
        self._server = server
        self._hwSensors =  hwsensors

        gApi.add_resource(SensorsView,'/sensor/<int:id>',resource_class_kwargs={'hwsensors':self._hwSensors,},methods=['GET'])
        gApi.add_resource(SensorsView,'/sensor/<int:id>/<int:type>',resource_class_kwargs={'app':self,},methods=['PUT'])
        #gApi.add_resource(RelaisView,'/relais/<int:id>',resource_class_kwargs={'app':self,},methods=['GET','DELETE'])
        #gApi.add_resource(MotorsView,'/motor/<int:id>',resource_class_kwargs={'app':self,},methods=['GET','DELETE'])

        gApi.init_app(self._server)
        logging.info("__init__ of API done")

class WebServiceResource(Resource):

        def __init__(self,api=None,*args,**kwargs):
            super().__init__(api,args,kwargs)
            logging.info('init got args: %s , kwargs: %s '%(args,kwargs))
            logging.info('resource created')


@gApi.doc(response={'404':'bad channel number'})
class SensorsView(WebServiceResource):
    """
    Sensor class.

    ** GET ** to read the sensor state
    ** PUT ** change the sensor type
    """
    #methods = ["GET","PUT"]
    def __init__(self,api=None,*args,**kwargs):
        super().__init__(api,args,kwargs)
        logging.info('init got args: %s , kwargs: %s '%(args,kwargs))
        self._hwSensors = kwargs['hwsensors']
        logging.info('sensor resource created')

    def get(self,id):
        """
        Read a sensor.
        ** GET ** to read the sensors value.
        :param int id: number of channel
        returns: str
        """
        logging.debug('get sensor: %d'%id)
        buffer = self._hwSensors[id-1].get_json()
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
