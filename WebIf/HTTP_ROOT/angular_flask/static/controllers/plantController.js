'use strict';

angular.module('wateringApp')
.controller('PlantListCtrl', ['$scope', 'PlantsFactory', 'SensorFactory', 'MotorFactory', '$state', 
  function ($scope, PlantsFactory, SensorFactory, MotorFactory, $state) {

      // callback for ng-click 'editPlant':
      $scope.editPlant = function (plantId) {
    	  $state.go('plant-detail', {id: plantId});
      };
      
      // callback for ng-click 'deletePlant':
      $scope.deletePlant = function (plantId) {
    	  $state.go('plant-delete', {id: plantId});
      };

      // callback for ng-click 'createPlant':
      $scope.createNewPlant = function () {
    	  $state.go('plant-creation');
      };

      $scope.plants = PlantsFactory.query();
  }])
.controller('PlantDetailCtrl', ['$scope', 'PlantFactory', 'PlantsFactory', 'SensorsFactory', 'MotorsFactory', '$state', '$stateParams',
  function ($scope, PlantFactory, PlantsFactory, SensorsFactory, MotorsFactory, $state, $stateParams) {

      // callback for ng-click 'updatePlant':
      $scope.updatePlant = function () {
    	  PlantFactory.update($scope.plant);
    	  $scope.plants = PlantsFactory.query();
    	  $state.go('plant-list');
      };

      // callback for ng-click 'cancel':
      $scope.cancel = function () {
          $state.go('plant-list');
      };

      $scope.plant = PlantFactory.show({id: $stateParams.id});
      
      $scope.sensors = SensorsFactory.query();
      $scope.motors = MotorsFactory.query();
  }])
.controller('PlantCreationCtrl', ['$scope', 'PlantsFactory', 'SensorsFactory', 'MotorsFactory', '$state',
  function ($scope, PlantsFactory, SensorsFactory, MotorsFactory ,$state) {

      // callback for ng-click 'createNewPlant':
      $scope.createNewPlant = function () {
    	  PlantsFactory.create($scope.plant);
    	  $scope.plants = PlantsFactory.query();
    	  $state.go('plant-list');
      }
      
      // callback for ng-click 'cancel':
      $scope.cancel = function () {
          $state.go('plant-list');
      };
      
      $scope.sensors = SensorsFactory.query();
      $scope.motors = MotorsFactory.query();
  }])
.controller('PlantDeleteCtrl', ['$scope', 'PlantFactory', 'PlantsFactory', '$state', '$stateParams',
  function ($scope, PlantFactory, PlantsFactory, $state, $stateParams) {

	  // callback for ng-click 'deletePlant':
      $scope.deletePlant = function (plantId) {
    	  PlantFactory.delete({ id: plantId });
    	  $scope.plants = PlantsFactory.query();
    	  $state.go('plant-list');
      };

	  // callback for ng-click 'cancel':
	  $scope.cancel = function () {
	      $state.go('plant-list');
	  };
	  
	  $scope.plant = PlantFactory.show({id: $stateParams.id});
  }]);