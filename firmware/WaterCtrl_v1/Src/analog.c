/*
 * analog.c
 *
 *  Created on: Jun 29, 2016
 *      Author: micha
 */


#include <stdlib.h>
#include <stdbool.h>

#include <cmsis_os.h>

#include "main.h"
#include "adc.h"
#include "sensor.h"

#include "analog.h"

ADC_HandleTypeDef *_adc;

void initAnalogMeasurement(ADC_HandleTypeDef *adc)
{
	if (NULL != adc)
		_adc = adc;
	else
		E("NULL pointer given");
}

int16_t getAnalogValue(uint8_t sensor){
	int16_t value = 0;

	/* set sensor channel */
	SetSensorChannel(sensor-1);
	osDelay(200);
	/* start adc */
	HAL_ADC_Start(_adc);
	/* read value after delay */
	if (HAL_ADC_PollForConversion(_adc,200) != HAL_OK)
	{
		uint adcState = HAL_ADC_GetState(_adc);
		uint adcError = HAL_ADC_GetError(_adc);
		E("Failed to poll for adc, state: %d error: %d",adcState,adcError);
	}
	else
	{
		value = 1000 -( (HAL_ADC_GetValue(_adc)*1000)/4096);
	}
	/* stop adc */
	HAL_ADC_Stop(_adc);

	/* clear sensor channel */
	ClearSensorChannel();

	return value;
}
