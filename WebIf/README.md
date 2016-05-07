This is for the web interface.

I don't want to stress the ESP8266 WiFi module,
thats why i want to implement this an a server.

The idea is to use a Python script on a server that does the following:
- [ ] Implement a webserver
  - [ ] Provide nice info graphics
  - [ ] Provide control interface
  - [ ] Provide debugging output
- [ ] Receive data from WaterCtrl and save it to database
- [ ] Send control signals to WaterCtrl

Provided REST API via the waterctrl-rest.py file.
You need to install some Python modules via pip:
- [ ] pip install flask
- [ ] pip install flask-sqlalchemy
- [ ] pip install Flask-Restless
- [ ] pip install mysql-python

