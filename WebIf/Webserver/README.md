# AngularJS + Flask WaterCtrl App 

Angular frontend and a Flask / python backend.

### How to Get Started

1. clone this repo

2. install all the necessary packages
> pip install -r requirements.txt

3. configure your database in angular_flask/settings.py

4. run the app
> python runserver.py

5. create and seed the db (the server must still be running, so open a new terminal window first)
> python manage.py create_db
> python manage.py seed_db --seedfile 'data/db_user.json'
> python manage.py seed_db --seedfile 'data/db_log.json'
> python manage.py seed_db --seedfile 'data/db_motor.json'
> python manage.py seed_db --seedfile 'data/db_sensor.json'
> python manage.py seed_db --seedfile 'data/db_plant.json'
> python manage.py seed_db --seedfile 'data/db_watering.json'
> python manage.py seed_db --seedfile 'data/db_sensor_response.json'

6. provided password is '1234', encrypted with CryptoJS and secret key: '#WaterCtrlS3cret'

7. check out your site
> http://localhost:80/

8. if you like this project, give it a star :)
