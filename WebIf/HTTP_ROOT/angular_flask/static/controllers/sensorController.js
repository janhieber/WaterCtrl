'use strict';

angular.module('wateringApp')
.controller('SensorListCtrl', ['$scope', 'SensorsFactory', '$state', 
  function ($scope, SensorsFactory, $state) {

      // callback for ng-click 'editSensor':
      $scope.editSensor = function (sensorId) {
    	  $state.go('sensor-detail', {id: sensorId});
      };
      
      // callback for ng-click 'deleteSensor':
      $scope.deleteSensor = function (sensorId) {
    	  $state.go('sensor-delete', {id: sensorId});
      };

      // callback for ng-click 'createSensor':
      $scope.createNewSensor = function () {
    	  $state.go('sensor-creation');
      };

      $scope.sensors = SensorsFactory.query();
  }])
.controller('SensorDetailCtrl', ['$scope', 'SensorFactory', 'SensorsFactory', '$state', '$stateParams',
  function ($scope, SensorFactory, SensorsFactory, $state, $stateParams) {

      // callback for ng-click 'updateSensor':
      $scope.updateSensor = function () {
      	  SensorFactory.update($scope.sensor);
      	  $scope.sensors = SensorsFactory.query();
      	  $state.go('sensor-list');
      };

      // callback for ng-click 'cancel':
      $scope.cancel = function () {
          $state.go('sensor-list');
      };

      $scope.sensor = SensorFactory.show({id: $stateParams.id});
  }])
.controller('SensorCreationCtrl', ['$scope', 'SensorsFactory', '$state',
  function ($scope, SensorsFactory, $state) {

      // callback for ng-click 'createNewSensor':
      $scope.createNewSensor = function () {
      	  SensorsFactory.create($scope.sensor);
      	  $scope.sensors = SensorsFactory.query();
    	  $state.go('sensor-list');
      }
      
      // callback for ng-click 'cancel':
      $scope.cancel = function () {
          $state.go('sensor-list');
      };
  }])
.controller('SensorDeleteCtrl', ['$scope', 'SensorFactory', 'SensorsFactory', '$state', '$stateParams',
  function ($scope, SensorFactory, SensorsFactory, $state, $stateParams) {

	  // callback for ng-click 'deleteSensor':
      $scope.deleteSensor = function (sensorId) {
    	  SensorFactory.delete({ id: sensorId });
    	  $scope.sensors = SensorsFactory.query();
      	  $state.go('sensor-list');
      };

	  // callback for ng-click 'cancel':
	  $scope.cancel = function () {
	      $state.go('sensor-list');
	  };
	  
	  $scope.sensor = SensorFactory.show({id: $stateParams.id});
  }]);