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
 * This structure hold all the variables necessary for communication with the sensor
 */
typedef struct{
	int16_t temp;
	int16_t hum;
	uint8_t				bitsRX[5];
	//float 				temp;
	//float				hum;
	uint8_t				crcErrorFlag;
	DHT22_STATE			state;
	TIM_HandleTypeDef*	timHandle;
	TIM_IC_InitTypeDef	timICHandle;
	uint32_t			timChannel;
	uint16_t			gpioPin;
	GPIO_TypeDef*		gpioPort;
	int					bitPos;
	IRQn_Type			timerIRQn;
	uint32_t			gpioAlternateFunction;
	uint16_t			lastVal;
	void				(*errorCallback)(DHT22_RESULT);
} DHT22_HandleTypeDef;

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

int32_t getDHT22_Temperature(uint8_t sensor);
int32_t getDHT22_Humidity(uint8_t sensor);

#endif /* INC_DHT22_H_ */
