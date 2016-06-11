/*
 * dht22.h
 *
 *  Created on: Jan 3, 2015
 *      Author: peter
 *
 *  https://github.com/petoknm/DHT22.git
 *
 *
 */

#ifndef INC_DHT22_H_
#define INC_DHT22_H_

#include "stm32f1xx_hal.h"

/**
 * Possible return values of the functions
 */
typedef enum{
	DHT22_OK,
	DHT22_ERROR,
	DHT22_CRC_ERROR
} DHT22_RESULT;

/**
 * Describes the state of the sensor
 */
typedef enum{
	DHT22_RECEIVED,		/*< Data have been received but not yet processed */
	DHT22_RECEIVING,	/*< Data from sensor are currently being received */
	DHT22_READY	/*< The sensor has finished all operations and is in wait state */
} DHT22_STATE;


/**
 * Initializes the DHT22 communication
 * @param	handle - a pointer to the DHT22 handle
 * @return	whether the function was successful or not
 */
DHT22_RESULT DHT22_Init(TIM_HandleTypeDef* handle);

/**
 * Deinitializes the DHT22 communication
 * @param	handle - a pointer to the DHT22 handle
 * @return	whether the function was successful or not
 */
DHT22_RESULT DHT22_DeInit(TIM_HandleTypeDef* handle);

uint32_t getDHT22_Temperature();
uint32_t getDHT22_Humidity();

#endif /* INC_DHT22_H_ */
