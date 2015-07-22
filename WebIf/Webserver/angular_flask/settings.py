
DEBUG = True
SECRET_KEY = 'temporary_secret_key'  # make sure to change this

DBUSER = 'waterctrl_user'
DBPASSWD = 'W@t3rC!rl'
DBSCHEMA = 'waterctrl'
DBHOST = '127.0.0.1'

SQLALCHEMY_DATABASE_URI = 'mysql://' + DBUSER + ':' + DBPASSWD + '@' + DBHOST + '/' + DBSCHEMA
