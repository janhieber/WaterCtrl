import threading
import queue
import logging
import os
import time

from flask.views import MethodView

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
        self._app.setRelais(id)
        return str('hello world %d' % id)

    def delete(self,id):
        logging.info("del function, id= %d",id)
        self._app.clearRelais(id)
        return str('cleared realis %d' % id)

class app(threading.Thread):
    SPI_SET_RELAIS = [0x17, 0x00, 0x01, 0x00, 0x00, 0x00]
    SPI_CLEAR_RELAIS = [0x17, 0x00, 0x00, 0x00, 0x00, 0x00]


    def __init__(self,server,sendQueue):
        threading.Thread.__init__(self)
        self.exit = False
        self._sendQueue = sendQueue
        self._relais = RelaisView(self)
        server.add_url_rule('/relais/<int:id>',view_func=self._relais.as_view('relais',self))

    def run(self):
        logging.info("starting the web server on port 5372")

        while True:
            time.sleep(100)

        logging.error("WebService run left")

    def setRelais(self,id):
        self.SPI_SET_RELAIS[1] = id
        logging.debug('setRelais #%d' % id)
        self._sendQueue.put(self.SPI_SET_RELAIS)

    def clearRelais(self,id):
        self.SPI_CLEAR_RELAIS[1] = id
        logging.debug('clearRelais #%d' % id)
        self._sendQueue.put(self.SPI_CLEAR_RELAIS)
