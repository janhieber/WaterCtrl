/*
 * dht22.c
 *
 *  Created on: Jan 3, 2015
 *      Author: peter
 *
 *  https://github.com/petoknm/DHT22.git
 *
 *
 */

#include <cmsis_os.h>

#include "mxconstants.h"
#include "dht22.h"

#define SYSNAME "DHT22"

/**
 * This structure hold all the variables necessary for communication with the sensor
 */
typedef struct{
	uint8_t				bitsRX[5];
	float 				temp;
	float				hum;
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
 * Reads the current temperature and humidity from the sensor
 * @param	handle - a pointer to the DHT22 handle
 * @return	whether the function was successful or not
 */
DHT22_RESULT DHT22_ReadData(DHT22_HandleTypeDef* handle);

/**
 * Handles the pin interrupt
 * @param	handle - a pointer to the DHT22 handle
 */
void DHT22_InterruptHandler(DHT22_HandleTypeDef* handle);

/**
 * Initiates a transfer of sensor data
 * @param	handle - a pointer to the DHT22 handle
 */
DHT22_RESULT DHT22_InitiateTransfer(DHT22_HandleTypeDef* handle);

/**
 * Sets up the pin as an output
 * @param	handle - a pointer to the DHT22 handle
 */
void DHT22_SetPinOUT(DHT22_HandleTypeDef* handle);

/**
 * Sets up the pin as an input
 * @param	handle - a pointer to the DHT22 handle
 */
void DHT22_SetPinIN(DHT22_HandleTypeDef* handle);

DHT22_HandleTypeDef dht;

#define DHT22_StartIT()	if(HAL_TIM_IC_Start_IT(handle->timHandle, handle->timChannel) != HAL_OK) return DHT22_ERROR
#define DHT22_StopIT()	if(HAL_TIM_IC_Stop_IT(handle->timHandle, handle->timChannel) != HAL_OK) return DHT22_ERROR

void DHT22_SetPinOUT(DHT22_HandleTypeDef* handle) {
	HAL_NVIC_DisableIRQ(handle->timerIRQn);
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin = handle->gpioPin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
	HAL_GPIO_Init(handle->gpioPort, &GPIO_InitStruct);
}

void DHT22_SetPinIN(DHT22_HandleTypeDef* handle) {
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin = handle->gpioPin;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
	//GPIO_InitStruct.Alternate = handle->gpioAlternateFunction;
	HAL_GPIO_Init(handle->gpioPort, &GPIO_InitStruct);
	HAL_NVIC_EnableIRQ(handle->timerIRQn);
	HAL_NVIC_SetPriority(handle->timerIRQn, 0, 0);
}

DHT22_RESULT DHT22_Init(TIM_HandleTypeDef* handle) {

	INITBEGIN;
	dht.gpioPin = FREQ_Pin;
	dht.gpioPort = FREQ_GPIO_Port;
	dht.timChannel = TIM_CHANNEL_3;
	dht.timHandle = handle;
	//dht.timHandle.Instance = TIM3;

#if 0
	dht.timHandle->Init.Period = 0xFFFF;
	dht.timHandle.Init.Prescaler = 0;
	dht.timHandle.Init.ClockDivision = 0;
	dht.timHandle.Init.CounterMode = TIM_COUNTERMODE_UP;
	if (HAL_TIM_IC_Init(&dht.timHandle) != HAL_OK) {
		return DHT22_ERROR;
	}
	dht.timICHandle.ICPolarity = TIM_ICPOLARITY_FALLING;
	dht.timICHandle.ICSelection = TIM_ICSELECTION_DIRECTTI;
	dht.timICHandle.ICPrescaler = TIM_ICPSC_DIV1;
	dht.timICHandle.ICFilter = 0;
	if (HAL_TIM_IC_ConfigChannel(&dht.timHandle, &dht.timICHandle,
			dht.timChannel) != HAL_OK) {
		return DHT22_ERROR;
	}
#endif
	INITEND;
	return DHT22_OK;
}

DHT22_RESULT DHT22_DeInit(TIM_HandleTypeDef* handle) {
	return DHT22_OK;
}

DHT22_RESULT DHT22_InitiateTransfer(DHT22_HandleTypeDef* handle) {

	DHT22_SetPinOUT(handle);
	HAL_GPIO_WritePin(handle->gpioPort, handle->gpioPin, GPIO_PIN_RESET);
	osDelay(2);
	handle->bitPos = -1;
	for (int i = 0; i < 5; i++) {
		handle->bitsRX[i] = 0;
	}

	DHT22_SetPinIN(handle);

	handle->state = DHT22_RECEIVING;
	DHT22_StartIT();

	return DHT22_OK;
}

void DHT22_InterruptHandler(DHT22_HandleTypeDef* handle) {

	uint16_t val = HAL_TIM_ReadCapturedValue(handle->timHandle,
			handle->timChannel);

	uint32_t freq = HAL_RCC_GetPCLK2Freq();

	uint16_t val2;
	if (val > handle->lastVal)
		val2 = val - handle->lastVal;
	else
		val2 = 65535 + val - handle->lastVal;

	handle->lastVal = val;

	float time = 1000000.0 * val2 / freq;

	if (handle->bitPos < 0) {
		if (time > 155.0 && time < 165.0) {
			handle->bitPos = 0;
		}
	} else if (handle->bitPos >= 0 && handle->bitPos < 40) {
		if (time > 78.0 && time < 97.0) {
			handle->bitsRX[handle->bitPos / 8] &= ~(1
					<< (7 - handle->bitPos % 8));
			handle->bitPos++;
		} else if (time > 120.0 && time < 145.0) {
			handle->bitsRX[handle->bitPos / 8] |= 1 << (7 - handle->bitPos % 8);
			handle->bitPos++;
		} else {
			handle->bitPos = -1;
			HAL_TIM_IC_Stop_IT(handle->timHandle, handle->timChannel);
			handle->state = DHT22_READY;
		}
	}

	if(handle->bitPos==40){
		handle->bitPos = -1;
		HAL_TIM_IC_Stop_IT(handle->timHandle, handle->timChannel);
		uint8_t sum = 0;
		for (int i = 0; i < 4; i++) {
			sum += handle->bitsRX[i];
		}
		if (sum == handle->bitsRX[4]) {
			handle->crcErrorFlag = 0;

			int16_t temp10 = 0;
			if ((handle->bitsRX[2] & 0x80) == 0x80) {
				temp10 |= (handle->bitsRX[2] & 0x7F) << 8;
				temp10 |= handle->bitsRX[3];
				temp10 *= -1;
			} else {
				temp10 |= handle->bitsRX[2] << 8;
				temp10 |= handle->bitsRX[3];
			}
			handle->temp = 0.1 * temp10;

			int16_t hum10 = 0;
			if ((handle->bitsRX[0] & 0x80) == 0x80) {
				hum10 |= (handle->bitsRX[0] & 0x7F) << 8;
				hum10 |= handle->bitsRX[1];
				hum10 *= -1;
			} else {
				hum10 |= handle->bitsRX[0] << 8;
				hum10 |= handle->bitsRX[1];
			}
			handle->hum = 0.1 * hum10;
		} else {
			handle->crcErrorFlag = 1;
		}
		handle->state = DHT22_RECEIVED;
	}

}

DHT22_RESULT DHT22_ReadData(DHT22_HandleTypeDef* handle) {

	DHT22_InitiateTransfer(handle);

	osDelay(1000);

	if(handle->crcErrorFlag==1){
		return DHT22_CRC_ERROR;
	}
	if(handle->state!=DHT22_RECEIVED){
		return DHT22_ERROR;
	}else{
		handle->state=DHT22_READY;
	}
	return DHT22_OK;
}


uint32_t getDHT22_Temperature() {
	DHT22_ReadData(&dht);
	return dht.temp*(uint32_t)10;
}

uint32_t getDHT22_Humidity() {
	DHT22_ReadData(&dht);
	return dht.temp*(uint32_t)10;
}
