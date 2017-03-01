"""
SpiCom.py
"""
import threading
import queue
import time
import random
import logging
import configparser
import psycopg2
import sys
import datetime

def app()
__init__(type(self)=self,sendQueue,recvQueue):
    self._sendQueue=sendQueue
    self._recvQueue=recvQueue
    self._pendingRequest[] = 0;

def syncSendMessage(buffer):
    receiveBuffer[]
    ret=True
    if (0 == self._pendingRequest[0])
        self._pendingRequest = buffer
        self._sendQueue.put(buffer)
        # wait for response
        while(self._pendingRequest):
            self._recvQueue.get()
            time.sleep(0.1)
        self._pendingResponse
    else
        ret=False

    return ret

def run(self)
