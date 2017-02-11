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

int16_t getAnalogValue(uint8_t sensor, uint8_t type){
	int16_t value = 0;

	ADC_ChannelConfTypeDef sConfig;
	GPIO_InitTypeDef GPIO_InitStruct;

	if (1 == type)
	{
		sConfig.Channel = ADC_CHANNEL_8;

		GPIO_InitStruct.Pin = FREQ_Pin;
	}
	else
	{
		sConfig.Channel = ADC_CHANNEL_2;
		GPIO_InitStruct.Pin = FREQ_Pin;

	}

	GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
	HAL_GPIO_Init(ANALOG_GPIO_Port, &GPIO_InitStruct);

	sConfig.Rank = 1;
	sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;

	if (HAL_ADC_ConfigChannel(_adc, &sConfig) != HAL_OK)
	{
		Error_Handler();
	}

	/* set sensor channel */
	SetSensorChannel(sensor-1);
	osDelay(500);
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
		value = ( (HAL_ADC_GetValue(_adc)*3300)/4095);
	}
	/* stop adc */
	HAL_ADC_Stop(_adc);

	/* clear sensor channel */
	ClearSensorChannel();

	return value;
}
