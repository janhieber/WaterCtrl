#!/usr/bin/python3
# coding: utf8

import http.server
import time
import os
import sys
# import urllib.request
import mysql.connector

import mimetypes

import hashlib
import string
import http.cookies
# import http.client
import random
import urllib
import json
import logging
import inspect
import signal

# major and minor version numbers
VERSION = [0, 1]

### webserver config
PORT = 8000
IP = ''
ROOTDIR = 'www'

### database config (mysql/mariadb)
DBUSER = 'waterctrl'
DBPASSWD = str(sys.argv[1])
DB = 'waterctrl'
DBTYPE = 'socket'  #'socket' or 'host'
DBHOST = '127.0.0.1'
DBSOCKET = '/var/run/mysqld/mysqld.sock'

###################################################################################################


print("ATTENTION: THIS IS NOT TESTET, JUST A BUNCH OF CODE!!!!")
print("ATTENTION: THIS IS NOT TESTET, JUST A BUNCH OF CODE!!!!")
print("ATTENTION: THIS IS NOT TESTET, JUST A BUNCH OF CODE!!!!")
print("ATTENTION: THIS IS NOT TESTET, JUST A BUNCH OF CODE!!!!")
print("ATTENTION: THIS IS NOT TESTET, JUST A BUNCH OF CODE!!!!")


# for db
cnx = 0
cursor = 0

os.chdir(ROOTDIR)  # change to basedir
chars = string.ascii_letters + string.digits  # for random generator 
sessionDict = {}  # dictionary mapping session id's to session objects

### session element, store session vars
class SessionElement(object):
	pass

### generate random string
def generateRandom(length):
	return ''.join([random.choice(chars) for i in range(length)])

### own requesthandler
class MyHandler(http.server.BaseHTTPRequestHandler):
	def do_GET(self):
		# generate path
		try:
			if self.path == '/':
				self.fullpath = os.path.abspath(ROOTDIR + os.path.sep)
			else:
				path = os.path.abspath(os.path.join(ROOTDIR, self.path.lstrip('/')))
				self.fullpath = os.path.realpath(path)
		except:
			self.fullpath = os.path.abspath(ROOTDIR + os.path.sep)
		
		# deliver normal files, the directory comparison should be done better
		if self.fullpath == (ROOTDIR + os.path.sep) or
			(self.fullpath.startswith(ROOTDIR + os.path.sep)
			and os.path.isfile(self.fullpath)
			and not os.path.islink(self.fullpath)):

			# set default file when nothing is given
			if len(os.path.basename(self.fullpath)) < 1:
				self.fullpath = os.path.join(ROOTDIR, 'index.html')
			
			# send responsecode and content type
			self.send_response(200)
			
			# determine content type
			mtype = str(mimetypes.guess_type(self.fullpath)[0])
			if mtype.lower() == 'none':
				mtype = 'application/octet-stream'
			self.send_header('Content-type', mtype + '; charset=utf-8')
			
			# send data
			f = open(self.fullpath, 'rb')
			self.wfile.write(f.read())
			f.close()

		else:
			self.send_response(404)
			self.end_headers()
		
		return
			
	def do_POST(self):
		# parse post data
		length = int(self.headers['content-length'])
		postvars = urllib.parse.parse_qs(self.rfile.read(length).decode("utf-8"))
		for k, v in list(postvars.items()):
			postvars[k] = v[0].strip('[]')
		return
		
	### advertise the servers valid options to the browser
	def do_OPTIONS(self):
		self.send_response(200, "ok")
		self.send_header('Access-Control-Allow-Origin', '*')
		self.send_header('Access-Control-Allow-Methods', 'GET, POST, OPTIONS')
		self.send_header("Access-Control-Allow-Headers", "X-Requested-With")
		return

def dbCreateDatabase():
	pass

def dbConnect(pw):
	if len(pw) > 0:
		passwd = pw
	else:
		passwd = DBPASSWD

	if DBTYPE == 'socket':
		cnx = mysql.connector.connect(user=DBUSER, database=DB,
			password=DBPASSWD, unix_socket=DBSOCKET)
	else:
		cnx = mysql.connector.connect(user=DBUSER, database=DB,
			password=DBPASSWD, host=DBHOST)
	cursor = cnx.cursor()

def dbDisconnect():
	cnx.commit()
	cursor.close()
	cnx.close()


def main():
	dbConnect
	
	# parse commandline
	if sys.argv[1] == 'run':
		pass

	elif sys.argv[1] == 'createDatabase':
		# since the first param is not the SQL pw, ask for it!
		dbConnect('password')
		dbCreateDatabase()
		dbDisconnect()
		return

	elif sys.argv[1] == 'addUser':
		# setup logging
		logging.basicConfig(format='%(asctime)s %(levelname)s: %(message)s',
							datefmt='%Y-%m-%d %H:%M:%S', level=logging.DEBUG)
		dbConnect()
		print()
		firstname = input('first name: ');
		surname = input('surname: ');
		while True:
			password = input('password: ');
			if len(password) < 4: print('ERROR: password too short')
			else: break
		while True:
			email = input('Email: ');
			if len(email) < 5: print('ERROR: Email too short')
			else: break
		phone = input('phone number: ');
		try: permission = int(input('permission level [0]: '))
		except: permission = 0
		adduser(firstname=firstname, surname=surname, password=password,\
				email=email, phone=phone, permission=permission)
		dbClose()
		return
	else:
		print("WaterCtrl web interface version %i.%i" % (VERSION[0], VERSION[1]))
		print()
		print("parameters:")
		print("\trun\t\trun the application in server mode")
		print("\tcreateDatabase\tcreate database tables")
		print()
		return
	
	
	dbConnect()

	server = http.server.HTTPServer((IP, PORT), MyHandler)
	try:
		server.serve_forever()
	except KeyboardInterrupt:  # cancel on Ctrl+C
		pass
	
	server.server_close()
	dbDisconnect()



### catch SIGTERM and close db connection before killing
def signal_handler(signal, frame):
	dbDisconnect()
	sys.exit(0)


### if not called as module, execute main
if __name__ == '__main__':
	signal.signal(signal.SIGTERM, signal_handler)
	main()




