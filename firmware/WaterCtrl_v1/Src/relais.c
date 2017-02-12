/*
 * relais.c
 *
 *  Created on: Jan 1, 2017
 *      Author: micha
 */

#include "relais.h"
#define SYSNAME "Relais"
#define NUMBER_OF_PINS 5
const uint16_t _pinList[NUMBER_OF_PINS] = {
		RELAIS_0_Pin,
		RELAIS_1_Pin,
		RELAIS_2_Pin,
		RELAIS_3_Pin,
		RELAIS_4_Pin
};

GPIO_TypeDef * _portList[NUMBER_OF_PINS] = {
		RELAIS_0_GPIO_Port,
		RELAIS_1_GPIO_Port,
		RELAIS_2_GPIO_Port,
		RELAIS_3_GPIO_Port,
		RELAIS_4_GPIO_Port,
};

bool InitRelais()
{
	INITBEGIN;

	INITEND;
	return true;
}

bool SetRelais(eRelaisChannel channel)
{
	bool ret = true;
	uint16_t chan = 0;

	if( (0 < channel) && (NUMBER_OF_PINS >= channel))
	{
		chan = channel -1;
		if (GPIO_PIN_SET != HAL_GPIO_ReadPin(_portList[chan],_pinList[chan]))
			HAL_GPIO_WritePin(_portList[chan],_pinList[chan],GPIO_PIN_SET);
	}
	else
		ret = false;
	return ret;
}

bool ClearRelais(eRelaisChannel channel)
{
	bool ret = true;
	uint16_t chan = 0;

	if( (0 < channel) && (NUMBER_OF_PINS >= channel))
	{
		chan = channel -1;
		//if (GPIO_PIN_RESET != HAL_GPIO_ReadPin(_portList[chan],_pinList[chan]))
			HAL_GPIO_WritePin(_portList[chan],_pinList[chan],GPIO_PIN_RESET);
	}
	else
		ret = false;
	return ret;
}

bool ToggleRelais(eRelaisChannel channel)
{
	bool ret = true;
	uint16_t chan = 0;

	if( (0 < channel) && (NUMBER_OF_PINS >= channel))
	{
		chan = channel -1;
		HAL_GPIO_TogglePin(_portList[chan],_pinList[chan]);
	}
	else
		ret = false;
	return ret;
}

bool IsRelaisSet(eRelaisChannel channel)
{
	bool ret = true;
	uint16_t chan = 0;

	if( (0 < channel) && (NUMBER_OF_PINS >= channel))
	{
		chan = channel -1;
		if (GPIO_PIN_RESET != HAL_GPIO_ReadPin(_portList[chan],_pinList[chan]))
			ret = true;
		else
			ret = false;
	}
	else
		ret = false;
	return ret;
}
