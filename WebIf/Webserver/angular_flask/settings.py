
DEBUG = True
SECRET_KEY = 'temporary_secret_key'  # make sure to change this

DBUSER = 'waterctrl_user'
DBPASSWD = 'Wfmd22nGixsP'
DBSCHEMA = 'waterctrl'
DBHOST = '127.0.0.1'
DBPORT = '5432'

SQLALCHEMY_DATABASE_URI = 'postgresql://' + DBUSER + ':' + DBPASSWD + '@' + DBHOST + ":" + DBPORT + '/' + DBSCHEMA