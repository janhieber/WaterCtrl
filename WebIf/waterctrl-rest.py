#!/usr/bin/python3
# coding: utf8
import flask
import flask.ext.sqlalchemy
import flask.ext.restless

### HTTP config
HTTP_PORT = 5001

### database config (mysql)
DBUSER = 'user'
DBPASSWD = 'pass'
DBSCHEMA = 'waterctrl'
DBHOST = '127.0.0.1'

# Create the Flask application and the Flask-SQLAlchemy object.
app = flask.Flask(__name__)
app.config['DEBUG'] = False
app.config['SQLALCHEMY_DATABASE_URI'] = 'mysql://' + DBUSER + ':' + DBPASSWD + '@' + DBHOST + '/' + DBSCHEMA
db = flask.ext.sqlalchemy.SQLAlchemy(app)

@app.after_request
def after_request(response):
    response.headers.add('Access-Control-Allow-Origin', '*')
    response.headers.add('Access-Control-Allow-Headers', 'Content-Type,Authorization')
    response.headers.add('Access-Control-Allow-Methods', 'GET,PUT,POST,DELETE')
    return response

# Flask-SQLALchemy models
class User(db.Model):
  __tablename__ = 'users'
  id = db.Column(db.Integer, primary_key = True)
  username = db.Column(db.String(20))
  password = db.Column(db.String(20))

class Log(db.Model):
  __tablename__ = 'logs'
  id = db.Column(db.Integer, primary_key = True)
  log_entry = db.Column(db.String(20))
  log_state = db.Column(db.Integer)
  log_date = db.Column(db.DateTime)

class Sensor(db.Model):
  __tablename__ = 'sensors'
  id = db.Column(db.Integer, primary_key = True)
  channel = db.Column(db.String(20))
  frequency = db.Column(db.String(20)) 


# Create the database tables.
db.create_all()

# Create the Flask-Restless API manager.
manager = flask.ext.restless.APIManager(app, flask_sqlalchemy_db=db)

# Create API endpoints
# Custom searching for GET requests
# http://flask-restless.readthedocs.org/en/latest/searchformat.html#searchformat
manager.create_api(User, methods=['GET', 'POST', 'DELETE', 'PUT'])
manager.create_api(Log, methods=['GET', 'POST', 'DELETE', 'PUT'])
manager.create_api(Sensor, methods=['GET', 'POST', 'DELETE', 'PUT'])

# start the flask loop
app.run(port=HTTP_PORT)