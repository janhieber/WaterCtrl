'use strict';

angular.module('wateringApp')
.controller('MotorListCtrl', ['$scope', 'MotorsFactory', '$state', 
  function ($scope, MotorsFactory, $state) {

      // callback for ng-click 'editMotor':
      $scope.editMotor = function (motorId) {
    	  $state.go('motor-detail', {id: motorId});
      };
      
      // callback for ng-click 'deleteMotor':
      $scope.deleteMotor = function (motorId) {
    	  $state.go('motor-delete', {id: motorId});
      };

      // callback for ng-click 'createMotor':
      $scope.createNewMotor = function () {
    	  $state.go('motor-creation');
      };

      $scope.motors = MotorsFactory.query();
  }])
.controller('MotorDetailCtrl', ['$scope', 'MotorFactory', 'MotorsFactory', '$state', '$stateParams',
  function ($scope, MotorFactory, MotorsFactory, $state, $stateParams) {

      // callback for ng-click 'updateMotor':
      $scope.updateMotor = function () {
      	  MotorFactory.update($scope.motor);
      	  $scope.motors = MotorsFactory.query();
    	  $state.go('motor-list');
      };

      // callback for ng-click 'cancel':
      $scope.cancel = function () {
          $state.go('motor-list');
      };

      $scope.motor = MotorFactory.show({id: $stateParams.id});
  }])
.controller('MotorCreationCtrl', ['$scope', 'MotorsFactory', '$state',
  function ($scope, MotorsFactory, $state) {

      // callback for ng-click 'createNewMotor':
      $scope.createNewMotor = function () {
      	  MotorsFactory.create($scope.motor);
      	  $scope.motors = MotorsFactory.query();
  	      $state.go('motor-list');
      }
      
      // callback for ng-click 'cancel':
      $scope.cancel = function () {
          $state.go('motor-list');
      };
  }])
.controller('MotorDeleteCtrl', ['$scope', 'MotorFactory', 'MotorsFactory', '$state', '$stateParams',
  function ($scope, MotorFactory, MotorsFactory, $state, $stateParams) {

	  // callback for ng-click 'deleteMotor':
      $scope.deleteMotor = function (motorId) {
    	  MotorFactory.delete({ id: motorId });
    	  $scope.motors = MotorsFactory.query();
    	  $state.go('motor-list');
      };

	  // callback for ng-click 'cancel':
	  $scope.cancel = function () {
	      $state.go('motor-list');
	  };
	  
	  $scope.motor = MotorFactory.show({id: $stateParams.id});
  }]);