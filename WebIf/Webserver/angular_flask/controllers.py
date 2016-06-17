import os

from flask import Flask, request, Response
from flask import render_template, url_for, redirect, send_from_directory
from flask import send_file, make_response, abort, jsonify

from angular_flask import app

# routing for API endpoints, generated from the models designated as API_MODELS
from angular_flask.core import api_manager
from angular_flask.models import *

for model_name in app.config['API_MODELS']:
    model_class = app.config['API_MODELS'][model_name]
    api_manager.create_api(model_class, methods=['GET', 'POST', 'PUT', 'DELETE'], results_per_page=200, max_results_per_page=200)

session = api_manager.session


# routing for basic pages (pass routing onto the Angular app)
@app.route('/')
#@app.route('/about')
#@app.route('/blog')
def basic_pages(**kwargs):
    return make_response(open('angular_flask/templates/index.html').read())


# routing for CRUD-style endpoints
# passes routing onto the angular frontend if the requested resource exists
from sqlalchemy.sql import exists

crud_url_models = app.config['CRUD_URL_MODELS']

@app.route('/api/sensor_responses/<date_from>/<date_to>', methods=['GET'])
def sensor_responses(date_from, date_to):
  if request.method == 'GET':
    results = SensorResponse.query.filter(SensorResponse.measure_date <= date_to).filter(SensorResponse.measure_date >= date_from).order_by(SensorResponse.measure_date.desc())
    #results = SensorResponse.query.filter(SensorResponse.measure_date.between(date_from, date_to))
    #results = SensorResponse.query.limit(100).offset(0).all()

    json_results = []
    for result in results:
      d = {'sensor_channel': result.sensor_channel,
           'frequency': result.frequency,
           'measure_date': result.measure_date}
      json_results.append(d)

    return jsonify(objects=json_results)



@app.route('/<model_name>/')
@app.route('/<model_name>/<item_id>')
def rest_pages(model_name, item_id=None):
    if model_name in crud_url_models:
        model_class = crud_url_models[model_name]
        if item_id is None or session.query(exists().where(
                model_class.id == item_id)).scalar():
            return make_response(open(
                'angular_flask/templates/index.html').read())
    abort(404)


# special file handlers and error handlers
@app.route('/favicon.ico')
def favicon():
    return send_from_directory(os.path.join(app.root_path, 'static'),
                               'img/favicon.ico')


@app.errorhandler(404)
def page_not_found(e):
    return render_template('404.html'), 404
