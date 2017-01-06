import threading
import queue
import logging
import os

from flask import Flask
from flask_restplus import Api, Resource

"""
This is the web service class that runs a REST. The REST can be used to direct
control the mainboards functions on STM32 over SPI.
"""

""" global vars """
api = Api()

class app(threading.Thread):
    def __init__(self,server):
        threading.Thread.__init__(self)
        self.exit = False
        api.init_app(server)

    def run(self):
        logging.info("starting the web server on port 5372")


@api.route('/relais/<int:id>',)
class HelloWorld(Resource):
    def get(self,id):
        logging.info(id)
        return {'hello': 'world %d' % id}
