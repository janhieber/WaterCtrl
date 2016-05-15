'use strict';

angular.module('wateringApp')
.factory('SensorsFactory', function ($resource) {
    return $resource('/api/sensor', {}, {
        query: { method: 'GET' },
        create: { method: 'POST' }
    })
})
.factory('SensorFactory', function ($resource) {
    return $resource('/api/sensor/:id', {}, {
        show: { method: 'GET' },
        update: { method: 'PUT', params: {id: '@id'} },
        delete: { method: 'DELETE', params: {id: '@id'} }
    })
})
.factory('MotorsFactory', function ($resource) {
    return $resource('/api/motor', {}, {
        query: { method: 'GET' },
        create: { method: 'POST' }
    })
})
.factory('MotorFactory', function ($resource) {
    return $resource('/api/motor/:id', {}, {
        show: { method: 'GET' },
        update: { method: 'PUT', params: {id: '@id'} },
        delete: { method: 'DELETE', params: {id: '@id'} }
    })
})
.factory('PlantsFactory', function ($resource) {
    return $resource('/api/plant', {}, {
        query: { method: 'GET' },
        create: { method: 'POST' }
    })
})
.factory('PlantFactory', function ($resource) {
    return $resource('/api/plant/:id', {}, {
        show: { method: 'GET' },
        update: { method: 'PUT', params: {id: '@id'} },
        delete: { method: 'DELETE', params: {id: '@id'} }
    })
})
.factory('LogsFactory', function ($resource) {
    return $resource('/api/log', {}, {
        query: { method: 'GET' },
        create: { method: 'POST' }
    })
})
.factory('LogFactory', function ($resource) {
    return $resource('/api/log/:id', {}, {
        show: { method: 'GET' },
        update: { method: 'PUT', params: {id: '@id'} },
        delete: { method: 'DELETE', params: {id: '@id'} }
    })
})
.factory('WateringsFactory', function ($resource) {
    return $resource('/api/watering', {}, {
        query: { method: 'GET' },
        create: { method: 'POST' }
    })
})
.factory('WateringFactory', function ($resource) {
    return $resource('/api/watering/:id', {}, {
        show: { method: 'GET' },
        update: { method: 'PUT', params: {id: '@id'} },
        delete: { method: 'DELETE', params: {id: '@id'} }
    })
});