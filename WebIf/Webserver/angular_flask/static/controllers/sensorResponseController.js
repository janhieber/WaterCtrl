'use strict';

angular.module('wateringApp')
.controller("SensorResponseCtrl", ['$scope', 'SensorResponsesFactory', '$state', 
	function ($scope, SensorResponsesFactory, $state) {

		$scope.sensor_responses = SensorResponsesFactory.query();
	}]);