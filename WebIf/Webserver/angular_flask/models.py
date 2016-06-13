from datetime import datetime

from angular_flask.core import db
from angular_flask import app

class User(db.Model):
	id = db.Column(db.Integer, primary_key = True)
	password = db.Column(db.String(60))
	
	def __init__(self, password):
		self.password = password

class Log(db.Model):
	id = db.Column(db.Integer, primary_key = True)
	log_entry = db.Column(db.String(200))
	log_state = db.Column(db.Integer)
	log_date = db.Column(db.DateTime)
	
	def __init__(self, log_entry, log_state, log_date=None):
		self.log_entry = log_entry
		self.log_state = log_state
		if log_date is None:
			log_date = datetime.utcnow()
		self.log_date = log_date

class Sensor(db.Model):
  sensor_channel = db.Column(db.Integer, primary_key = True, unique=True)
  frequency = db.Column(db.Integer)

  def __init__(self, sensor_channel, frequency):
    self.sensor_channel = sensor_channel
    self.frequency = frequency
    
class SensorResponse(db.Model):
    id = db.Column(db.Integer, primary_key = True)
    sensor_channel = db.Column(db.Integer)
    frequency = db.Column(db.Integer)
    measure_date =  db.Column(db.DateTime)
    
    @property
    def serialize(self):
      """Return object data in easily serializeable format"""
      return {
  	  'id'               : self.id,
      'sensor_channel'   : self.sensor_channel,
      'frequency'        : self.frequency,
  	  'measure_date'    : dump_datetime(self.watering_date)
  	}

      def __init__(self, sensor_channel, frequency, measure_date=None):
        if measure_date is None:
          measure_date = datetime.utcnow()
        self.sensor_channel = sensor_channel
        self.frequency = frequency

class Motor(db.Model):
  motor_channel = db.Column(db.Integer, primary_key = True, unique=True)
  duration = db.Column(db.Integer)
  sensor_channel = db.Column(db.Integer, db.ForeignKey('sensor.sensor_channel'))
  
  def __init__(self, motor_channel, duration, sensor_channel):
    self.motor_channel = motor_channel
    self.duration = duration
    self.sensor_channel = sensor_channel

class Plant(db.Model):
  id = db.Column(db.Integer, primary_key = True)
  name = db.Column(db.String(20))
  description = db.Column(db.Text)
  watering_interval = db.Column(db.Integer)
  motor_channel = db.Column(db.Integer, db.ForeignKey('motor.motor_channel'))

  def __init__(self, name, description, watering_interval, motor_channel):
    self.name = name
    self.description = description
    self.watering_interval = watering_interval
    self.motor_channel = motor_channel

class Watering(db.Model):
  id = db.Column(db.Integer, primary_key = True)
  watering_date = db.Column(db.DateTime)
  watering_duration = db.Column(db.Integer)
  watering_state = db.Column(db.Integer)
  plant_id = db.Column(db.Integer, db.ForeignKey('plant.id'))

  @property
  def serialize(self):
    """Return object data in easily serializeable format"""
    return {
	  'id'               : self.id,
	  'watering_date'    : dump_datetime(self.watering_date),
	  'watering_duration': self.watering_duration,
	  'watering_state'   : self.watering_state,
	  'plant_id'         : self.plant_id
	  # This is an example how to deal with Many2Many relations
	  #'many2many'  : self.serialize_many2many
	}

    def __init__(self, watering_duration, watering_state, plant_id, watering_date=None):
      if watering_date is None:
        watering_date = datetime.utcnow()
      self.watering_date = watering_date
      self.watering_duration = watering_duration
      self.watering_state = watering_state
      self.plant_id = plant_id


# models for which we want to create API endpoints
app.config['API_MODELS'] = {'user': User, 'log': Log, 'sensor': Sensor, 'motor': Motor, 'plant': Plant, 'watering': Watering, 'sensor_response': SensorResponse}

# models for which we want to create CRUD-style URL endpoints,
# and pass the routing onto our AngularJS application
app.config['CRUD_URL_MODELS'] = {'user': User, 'log': Log, 'sensor': Sensor, 'motor': Motor, 'plant': Plant, 'watering': Watering, 'sensor_response': SensorResponse}
