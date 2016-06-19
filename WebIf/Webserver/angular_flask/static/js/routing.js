angular.module('wateringApp')
.config(['$stateProvider', '$urlRouterProvider',
    function($stateProvider, $urlRouterProvider) {

		// For any unmatched url, redirect to /
		$urlRouterProvider.otherwise("/");
  
		// Now set up the states
		$stateProvider
			.state('home', {
				url: "/",
				templateUrl: "static/templates/home.html"
			})
			// MOTOR
			.state('motor-list', {
				url: '/motor-list',
				templateUrl: 'static/templates/motor/motor-list.html',
				controller: 'MotorListCtrl'
			})
			.state('motor-detail', {
				url: '/motor-detail/:id',
				templateUrl: 'static/templates/motor/motor-detail.html',
				controller: 'MotorDetailCtrl'
			})
			.state('motor-creation', {
				url: "/motor-creation",
			    templateUrl: 'static/templates/motor/motor-creation.html', 
			    controller: 'MotorCreationCtrl'
			})
			.state('motor-delete', {
				url: "/motor-delete/:id",
				templateUrl: 'static/templates/motor/motor-delete.html', 
				controller: 'MotorDeleteCtrl'
			})
			// SENSOR
			.state('sensor-list', {
				url: '/sensor-list',
				templateUrl: 'static/templates/sensor/sensor-list.html',
				controller: 'SensorListCtrl'
			})
			.state('sensor-detail', {
				url: '/sensor-detail/:id',
				templateUrl: 'static/templates/sensor/sensor-detail.html',
				controller: 'SensorDetailCtrl'
			})
			.state('sensor-creation', {
				url: "/sensor-creation",
				templateUrl: 'static/templates/sensor/sensor-creation.html', 
				controller: 'SensorCreationCtrl'
			})
			.state('sensor-delete', {
				url: "/sensor-delete/:id",
				templateUrl: 'static/templates/sensor/sensor-delete.html', 
				controller: 'SensorDeleteCtrl'
			})
			// PLANT
			.state('plant-list', {
				url: '/plant-list',
				templateUrl: 'static/templates/plant/plant-list.html',
				controller: 'PlantListCtrl'
			})
			.state('plant-detail', {
				url: '/plant-detail/:id',
				templateUrl: 'static/templates/plant/plant-detail.html',
				controller: 'PlantDetailCtrl'
			})
			.state('plant-creation', {
				url: "/plant-creation",
				templateUrl: 'static/templates/plant/plant-creation.html', 
				controller: 'PlantCreationCtrl'
			})
			.state('plant-delete', {
				url: "/plant-delete/:id",
				templateUrl: 'static/templates/plant/plant-delete.html', 
				controller: 'PlantDeleteCtrl'
			})
			// SENSOR_RESPONSE
			.state('sensor-frequencies', {
				url: '/sensor-frequencies',
				templateUrl: 'static/templates/sensor-response/sensor-response.html',
				controller: 'SensorResponseCtrl'
			})
			// WATERING
			.state('watering', {
				url: "/watering",
				templateUrl: 'static/templates/watering/watering.html', 
				controller: 'WateringController'
			})
			// LOG
			.state('log-list', {
				url: "/log-list",
				templateUrl: 'static/templates/log/log-list.html', 
				controller: 'LogListCtrl'
			})
			;
	}
]);