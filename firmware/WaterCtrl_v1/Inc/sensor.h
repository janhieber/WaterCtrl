/*
 * sensor.h
 *
 *  Created on: Jun 11, 2016
 *      Author: micha
 */

#ifndef SENSOR_H_
#define SENSOR_H_

typedef enum _eSensorType {
	SENS_MOISTURE, // Frequency Input to PB0
	SENS_DHT22,    // DHT22 connected to PA3
	SENS_DHT22_TEMP, // DHT22 Temperature PA3
	SENS_ANALOG_PA2,   // ANALOG_Input PA2
	SENS_ANALOG_PA3   // ANALOG Input PA3
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
void ClearSensorChannel();

void InitSensors();

void DeinitSensors();

/*
 * isr functions
 */
extern void setSensorType(uint8_t);

#endif /* SENSOR_H_ */
