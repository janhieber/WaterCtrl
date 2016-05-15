/* ---------------------------------------------------------------------------
** This software is in the public domain, furnished "as is", without technical
** support, and with no warranty, express or implied, as to its usefulness for
** any purpose.
**
** moistureMeasure.h
** Header for moistureMeasure module
**
** Author: dropedout
** -------------------------------------------------------------------------*/

/** @addtogroup MoistureMeasure
  * @{
  */

#ifndef MOISTUREMEASURE_H
#define MOISTUREMEASURE_H

int initMoistureMeasure(TIM_HandleTypeDef*);

int deinitMoistureMeasure(void);

uint32_t getSensorFrequency(int sensor);

void printMoisture();

void MoistureTask();

typedef struct SensorCmd {
	uint8_t sensor;
	uint8_t value;
}stSensorCmd;

void procSensor(void const * argument);

extern osMessageQId sensorQueue;
extern osPoolId  sensorPool;

#endif // MOISTUREMEASURE_H

/**
  * @}
  */
