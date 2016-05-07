'use strict';

angular.module('wateringApp')
.controller("WateringController", ['$scope', 'WateringsFactory', '$http', 
	function($scope, WateringsFactory, $http) {
	
		$http.get("/api/watering").success(function(response) {
            $scope.waterings = response.objects;
            
            var plantArray = [];
            var plantLabelArray = [];
            var dateLabelArray = [];
            
            // split the result data for the different plants
            for(var i=0; i<$scope.waterings.length; i++) {
                var plant_id = parseInt($scope.waterings[i].plant_id);
                if(plantArray[plant_id-1] == null) {
                    plantLabelArray.push(plant_id);
					plantArray[plant_id-1] = new Array();
					plantArray[plant_id-1].push($scope.waterings[i]);
                } else {
                    plantArray[plant_id-1].push($scope.waterings[i]);
                }
            }
            
            // sort the arrays via the plant ids
            plantArray.sort();
            plantLabelArray.sort;
            
            // check all dates
			for(var i=0; i < $scope.waterings.length; i++){
				var d = Date.createFromMysql($scope.waterings[i].watering_date);
				if(!isInArray(d.toLocaleDateString(), dateLabelArray)) {
					dateLabelArray.push(d.toLocaleDateString());
				}
			}
            
            $scope.chartConfig = {
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
                    text: 'Watering Diagram'
                },
                xAxis: {
                    startOnTick: true,
                    endOnTick: true,
                    type: 'datetime',
                    categories: dateLabelArray,
                    dateTimeLabelFormats: {
                        month: '%b %e, %Y'
                    }
                },
                loading: false
            }
          
            var temp = [];
            
            // create json dataset
            for(var i=0; i < plantArray.length; i++) {
                var plantName;
                var wateringDataArray = [];
                
                if(plantArray[i] != null) {
                    for(var j=0; j < dateLabelArray.length; j++) {
                        var label = dateLabelArray[j];
                        var duration = 0;

                        for(var k=0; k < plantArray[i].length; k++) {
                            var watering = plantArray[i][k];
                            plantName = watering.plant_id;

                            var wateringDate = Date.createFromMysql(watering.watering_date).toLocaleDateString();
                            var wateringState = parseInt(watering.watering_state);

                            if(label === wateringDate && wateringState == 1) {
                                duration = duration + parseInt(watering.watering_duration);
                            }
                        }
                        wateringDataArray.push(duration);
                    }
                    
                    // add plant data to chart
                    $scope.chartConfig.series.push({
                        name: plantName,
                        data: wateringDataArray
                    });
                    
                }

            }
            
        });
	
	}
]);