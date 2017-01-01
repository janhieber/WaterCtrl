/*
 * analog.h
 *
 *  Created on: Jun 29, 2016
 *      Author: micha
 */

#ifndef ANALOG_H_
#define ANALOG_H_

#include "stm32f1xx_hal.h"

void initAnalogMeasurement(ADC_HandleTypeDef *adc);

int16_t getAnalogValue(uint8_t sensor, uint8_t type);


#endif /* ANALOG_H_ */
