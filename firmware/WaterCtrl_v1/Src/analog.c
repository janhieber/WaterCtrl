/*
 * analog.c
 *
 *  Created on: Jun 29, 2016
 *      Author: micha
 */


#include <stdlib.h>
#include <stdbool.h>

#include <cmsis_os.h>

#include "mxconstants.h"
#include "dht22.h"

#include "analog.h"



int16_t getAnalogValue(uint8_t sensor){
	int16_t value;

	/* start adc */

	/* read value after delay */

	/* stop adc */
	HAL_ADC_EnableBufferSensor_Cmd(0);


	return value;
}
