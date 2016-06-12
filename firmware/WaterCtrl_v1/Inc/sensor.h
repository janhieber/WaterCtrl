/*
 * sensor.h
 *
 *  Created on: Jun 11, 2016
 *      Author: micha
 */

#ifndef SENSOR_H_
#define SENSOR_H_

typedef enum _eSensorType {
	SENS_MOISTURE,
	SENS_DHT22
}eSensorType;

#define SEN_CHANNEL0_ACTIVE 0
#define SEN_CHANNEL1_ACTIVE 1
#define SEN_CHANNEL2_ACTIVE 2
#define SEN_CHANNEL3_ACTIVE 3
#define SEN_CHANNEL4_ACTIVE 4
#define SEN_CHANNEL5_ACTIVE 5
#define SEN_CHANNEL6_ACTIVE 6
#define SEN_CHANNEL7_ACTIVE 7

void procSensor(void const * argument);

extern osMessageQId sensorQueue;
extern osPoolId  sensorPool;

typedef struct SensorCmd {
	uint8_t cmd;
	uint8_t sensor;
	int16_t value1;
	int16_t value2;
}stSensorCmd;

/**
  * @brief  Set the multiplexer to correct channel.
  * @param  channel Channel to use
  * @retval return value
  */
void SetSensorChannel(int channel);

void InitSensors();

void DeinitSensors();

#endif /* SENSOR_H_ */
