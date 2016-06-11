/*
 * sensor.h
 *
 *  Created on: Jun 11, 2016
 *      Author: micha
 */

#ifndef SENSOR_H_
#define SENSOR_H_

void procSensor(void const * argument);

extern osMessageQId sensorQueue;
extern osPoolId  sensorPool;

void MoistureTask();

typedef struct SensorCmd {
	uint8_t sensor;
	uint16_t value;
}stSensorCmd;

#endif /* SENSOR_H_ */
