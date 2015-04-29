'use strict';

angular.module('wateringApp')
.controller('LogListCtrl', ['$scope', 'LogsFactory', '$state', 
	function ($scope, LogsFactory, $state) {
		$scope.logs = LogsFactory.query();
 	}
]);