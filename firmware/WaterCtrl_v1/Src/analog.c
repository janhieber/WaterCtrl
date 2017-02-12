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
		sConfig.Channel = ADC_CHANNEL_3;
		GPIO_InitStruct.Pin = DHT22_Pin;
	}
	else
	{
		sConfig.Channel = ADC_CHANNEL_2;
		GPIO_InitStruct.Pin = ANALOG_Pin;
	}

	GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;

	if (1 == type )
	{
		HAL_GPIO_Init(DHT22_GPIO_Port, &GPIO_InitStruct);
	}
	else
	{
		HAL_GPIO_Init(ANALOG_GPIO_Port, &GPIO_InitStruct);
	}

	sConfig.Rank = 1;
	sConfig.SamplingTime = ADC_SAMPLETIME_239CYCLES_5;

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
	if (HAL_ADC_PollForConversion(_adc,500) != HAL_OK)
	{
		uint adcState = HAL_ADC_GetState(_adc);
		uint adcError = HAL_ADC_GetError(_adc);
		E("Failed to poll for adc, state: %d error: %d",adcState,adcError);
	}
	else
	{
		value = ( (HAL_ADC_GetValue(_adc)*(uint32_t)3300)/(uint32_t)4095)*(uint32_t)1000/(uint32_t)3300;
		//value = HAL_ADC_GetValue(_adc);
	}
	/* stop adc */
	HAL_ADC_Stop(_adc);

	/* clear sensor channel */
	ClearSensorChannel();
	osDelay(500);

	return value;
}
