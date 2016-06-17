'use strict';

angular.module('wateringApp')
.controller("SensorResponseCtrl", ['$scope', 'SensorResponsesFactory', '$http', '$filter', 
	function ($scope, SensorResponsesFactory, $http, $filter) {
		
		$scope.format = 'yyyy-MM-dd';
		
		$scope.today = function() {
			$scope.dateFrom = $filter('date')(new Date(), $scope.format);
			$scope.dateTo = $filter('date')(new Date(), $scope.format);
		};
		$scope.today();

		$scope.clear = function () {
		    $scope.dateFrom = null;
			$scope.dateTo = null;
		};

		// Disable weekend selection
		$scope.disabled = function(date, mode) {
		    return ( mode === 'day' && ( date.getDay() === 0 || date.getDay() === 6 ) );
		};

		$scope.toggleMin = function() {
		    $scope.minDate = $scope.minDate ? null : new Date();
		};
		$scope.toggleMin();

		$scope.open1 = function($event) {
  		    $event.preventDefault();
  		    $event.stopPropagation();
		    $scope.opened1 = true;
		};

		$scope.open2 = function($event) {
		    $event.preventDefault();
		    $event.stopPropagation();
		    $scope.opened2 = true;
	    };

		$scope.dateOptions = {
		    formatYear: 'yy',
		    startingDay: 1
		};
		
        $scope.renderDiagram = function() {
			var dateFrom = Date.toMysqlFormat(Date.setHoursAndMinutes($scope.dateFrom, 0, 0));
			var dateTo = Date.toMysqlFormat(Date.setHoursAndMinutes($scope.dateTo, 23, 59));
			
			var requestLine = "/api/sensor_responses/" + dateFrom + "/" + dateTo;
      	  	$http.get(requestLine).success(function(response) {
				$scope.sensor_responses = response.objects;
			
	            var sensorArray = [];
	            var sensorLabelArray = [];
				var dateLabelArray = [];
			
	            // split the result data for the different sensors
	            for(var i=0; i<$scope.sensor_responses.length; i++) {
	                var sensor_channel = parseInt($scope.sensor_responses[i].sensor_channel);
	                if(sensorArray[sensor_channel-1] == null) {
	                    sensorLabelArray.push(sensor_channel);
						sensorArray[sensor_channel-1] = new Array();
						sensorArray[sensor_channel-1].push($scope.sensor_responses[i]);
	                } else {
	                    sensorArray[sensor_channel-1].push($scope.sensor_responses[i]);
	                }
	            }
			
	            // check all dates
				for(var i=0; i < $scope.sensor_responses.length; i++){
					var d = Date.createFromMysql($scope.sensor_responses[i].measure_date);
					if(!isInArray(d.toLocaleString(), dateLabelArray)) {
						dateLabelArray.push(d.toLocaleString());
					}
				}
			
		        $scope.sensorResponseConfig = {
		            chart: {
		            	type: 'spline',
		                zoomType: 'x'
		            },
		            tooltip: {
		            	style: {
		                	padding: 10,
		                    fontWeight: 'bold'
		                }
		            },
		            series: [],
		            title: {
		                text: 'Sensor Frequencies Diagram'
		            },
		            xAxis: {
		                type: 'datetime',
		                dateTimeLabelFormats: {
							second: '%H:%M:%S',
							minute: '%H:%M',
							hour: '%H:%M',
							day: '%e. %b',
							week: '%e. %b',
							month: '%b \'%y',
							year: '%Y'
		                },
						title: {
							text: 'Date'
						}
		            },
					yAxis: {
					    title: {
					    	text: 'Sensor Values (kHz)'
					    },
					    min: 0
					},
					plotOptions: {
						spline: {
					    	marker: {
					        	enabled: true
					        }
						}
					},
		            loading: false
		        }
			
	            // create json dataset
	            for(var i=0; i < sensorArray.length; i++) {
	                var sensorChannel;
	                var sensorResponseDataArray = [];
                
	                if(sensorArray[i] != null) {
	                    for(var j=0; j < sensorArray[i].length; j++) {
	                        var sensorResponse = sensorArray[i][j];
	                        sensorChannel = sensorResponse.sensor_channel;
						
							sensorResponseDataArray.push([Date.parse(sensorResponse.measure_date),sensorResponse.frequency]);
	                    }
	                }
				
	                // add plant data to chart
	                $scope.sensorResponseConfig.series.push({
	                    name: sensorChannel,
	                    data: sensorResponseDataArray
	                });
				}
			});
        };

		
		/*$http.get("/api/sensor_response").success(function(response) {
			$scope.sensor_responses = response.objects;
			
            var sensorArray = [];
            var sensorLabelArray = [];
			var dateLabelArray = [];
			
            // split the result data for the different sensors
            for(var i=0; i<$scope.sensor_responses.length; i++) {
                var sensor_channel = parseInt($scope.sensor_responses[i].sensor_channel);
                if(sensorArray[sensor_channel-1] == null) {
                    sensorLabelArray.push(sensor_channel);
					sensorArray[sensor_channel-1] = new Array();
					sensorArray[sensor_channel-1].push($scope.sensor_responses[i]);
                } else {
                    sensorArray[sensor_channel-1].push($scope.sensor_responses[i]);
                }
            }
			
            // check all dates
			for(var i=0; i < $scope.sensor_responses.length; i++){
				var d = Date.createFromMysql($scope.sensor_responses[i].measure_date);
				if(!isInArray(d.toLocaleString(), dateLabelArray)) {
					dateLabelArray.push(d.toLocaleString());
				}
			}
			
	        $scope.sensorResponseConfig = {
	            chart: {
	            	type: 'spline',
	                zoomType: 'x'
	            },
	            tooltip: {
	            	style: {
	                	padding: 10,
	                    fontWeight: 'bold'
	                }
	            },
	            series: [],
	            title: {
	                text: 'Sensor Frequencies Diagram'
	            },
	            xAxis: {
	                type: 'datetime',
	                dateTimeLabelFormats: {
						second: '%H:%M:%S',
						minute: '%H:%M',
						hour: '%H:%M',
						day: '%e. %b',
						week: '%e. %b',
						month: '%b \'%y',
						year: '%Y'
	                },
					title: {
						text: 'Date'
					}
	            },
				yAxis: {
				    title: {
				    	text: 'Sensor Values (kHz)'
				    },
				    min: 0
				},
				plotOptions: {
					spline: {
				    	marker: {
				        	enabled: true
				        }
					}
				},
	            loading: false
	        }
			
            // create json dataset
            for(var i=0; i < sensorArray.length; i++) {
                var sensorChannel;
                var sensorResponseDataArray = [];
                
                if(sensorArray[i] != null) {
                    for(var j=0; j < sensorArray[i].length; j++) {
                        var sensorResponse = sensorArray[i][j];
                        sensorChannel = sensorResponse.sensor_channel;
						
						sensorResponseDataArray.push([Date.parse(sensorResponse.measure_date),sensorResponse.frequency]);
                    }
                }
				
                // add plant data to chart
                $scope.sensorResponseConfig.series.push({
                    name: sensorChannel,
                    data: sensorResponseDataArray
                });
			}
			
		});*/

	}
]);