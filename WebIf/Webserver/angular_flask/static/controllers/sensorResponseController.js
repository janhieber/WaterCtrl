'use strict';

angular.module('wateringApp')
.controller("SensorResponseCtrl", ['$scope', 'SensorResponsesFactory', '$http', 
	function ($scope, SensorResponsesFactory, $http) {
		
		$http.get("/api/sensor_response").success(function(response) {
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
	            options: {
	                chart: {
	                    type: 'line',
	                    zoomType: 'x'
	                },
	                tooltip: {
	                    style: {
	                        padding: 10,
	                        fontWeight: 'bold'
	                    }
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
					tickInterval: 3600 * 1000,
					title: {
						text: 'Date'
					}
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
						var data = [];
						
                        var sensorResponse = sensorArray[i][j];
                        sensorChannel = sensorResponse.sensor_channel;
						
						var frequency = sensorResponse.frequency;
                        var measureDate = Date.createFromMysql(sensorResponse.measure_date).toLocaleString();
						
						data.push(measureDate);
						data.push(frequency);
						
						sensorResponseDataArray.push(data);
                    }
                }
				
                // add plant data to chart
                $scope.sensorResponseConfig.series.push({
					pointInterval: 3600 * 1000,
                    name: sensorChannel,
                    data: sensorResponseDataArray
                });
			}
			
		});

	}
]);