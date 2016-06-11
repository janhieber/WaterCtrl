angular.module('wateringApp')
.config(['$stateProvider', '$urlRouterProvider', 'USER_ROLES',
    function($stateProvider, $urlRouterProvider, USER_ROLES) {

		// For any unmatched url, redirect to /
		$urlRouterProvider.otherwise("/");
  
		// Now set up the states
		$stateProvider
			.state('home', {
				url: "/",
				templateUrl: "static/templates/home.html",
				data: {
					authorizedRoles: [USER_ROLES.admin]
				}
			})
			// MOTOR
			.state('motor-list', {
				url: '/motor-list',
				templateUrl: 'static/templates/motor/motor-list.html',
				controller: 'MotorListCtrl',
				data: {
					authorizedRoles: [USER_ROLES.admin]
				}
			})
			.state('motor-detail', {
				url: '/motor-detail/:id',
				templateUrl: 'static/templates/motor/motor-detail.html',
				controller: 'MotorDetailCtrl',
				data: {
					authorizedRoles: [USER_ROLES.admin]
				}
			})
			.state('motor-creation', {
				url: "/motor-creation",
			    templateUrl: 'static/templates/motor/motor-creation.html', 
			    controller: 'MotorCreationCtrl',
			    data: {
			    	authorizedRoles: [USER_ROLES.admin]
			    }
			})
			.state('motor-delete', {
				url: "/motor-delete/:id",
				templateUrl: 'static/templates/motor/motor-delete.html', 
				controller: 'MotorDeleteCtrl',
				data: {
					authorizedRoles: [USER_ROLES.admin]
				}
			})
			// SENSOR
			.state('sensor-list', {
				url: '/sensor-list',
				templateUrl: 'static/templates/sensor/sensor-list.html',
				controller: 'SensorListCtrl',
				data: {
					authorizedRoles: [USER_ROLES.admin]
				}
			})
			.state('sensor-detail', {
				url: '/sensor-detail/:id',
				templateUrl: 'static/templates/sensor/sensor-detail.html',
				controller: 'SensorDetailCtrl',
				data: {
					authorizedRoles: [USER_ROLES.admin]
				}
			})
			.state('sensor-creation', {
				url: "/sensor-creation",
				templateUrl: 'static/templates/sensor/sensor-creation.html', 
				controller: 'SensorCreationCtrl',
				data: {
					authorizedRoles: [USER_ROLES.admin]
				}
			})
			.state('sensor-delete', {
				url: "/sensor-delete/:id",
				templateUrl: 'static/templates/sensor/sensor-delete.html', 
				controller: 'SensorDeleteCtrl',
				data: {
					authorizedRoles: [USER_ROLES.admin]
				}
			})
			// PLANT
			.state('plant-list', {
				url: '/plant-list',
				templateUrl: 'static/templates/plant/plant-list.html',
				controller: 'PlantListCtrl',
				data: {
					authorizedRoles: [USER_ROLES.admin]
				}
			})
			.state('plant-detail', {
				url: '/plant-detail/:id',
				templateUrl: 'static/templates/plant/plant-detail.html',
				controller: 'PlantDetailCtrl',
				data: {
					authorizedRoles: [USER_ROLES.admin]
				}
			})
			.state('plant-creation', {
				url: "/plant-creation",
				templateUrl: 'static/templates/plant/plant-creation.html', 
				controller: 'PlantCreationCtrl',
				data: {
					authorizedRoles: [USER_ROLES.admin]
				}
			})
			.state('plant-delete', {
				url: "/plant-delete/:id",
				templateUrl: 'static/templates/plant/plant-delete.html', 
				controller: 'PlantDeleteCtrl',
				data: {
					authorizedRoles: [USER_ROLES.admin]
				}
			})
			// SENSOR_RESPONSE
			.state('sensor-frequencies', {
				url: '/sensor-frequencies',
				templateUrl: 'static/templates/sensor-response/sensor-response.html',
				controller: 'SensorResponseCtrl',
				data: {
					authorizedRoles: [USER_ROLES.admin]
				}
			})
			// WATERING
			.state('watering', {
				url: "/watering",
				templateUrl: 'static/templates/watering/watering.html', 
				controller: 'WateringController',
				data: {
					authorizedRoles: [USER_ROLES.admin]
				}
			})
			// LOG
			.state('log-list', {
				url: "/log-list",
				templateUrl: 'static/templates/log/log-list.html', 
				controller: 'LogListCtrl',
				data: {
					authorizedRoles: [USER_ROLES.admin]
				}
			})
			;
	}
]);