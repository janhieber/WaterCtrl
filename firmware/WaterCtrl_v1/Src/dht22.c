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

#include "main.h"
#include "dht22.h"
#include "sensor.h"

#define SYSNAME "DHT22"

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
void DHT22_InterruptHandler(TIM_HandleTypeDef* handle);

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
//	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
	HAL_GPIO_Init(handle->gpioPort, &GPIO_InitStruct);
}

void DHT22_SetPinIN(DHT22_HandleTypeDef* handle) {
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin = handle->gpioPin;
	//GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
	HAL_GPIO_Init(handle->gpioPort, &GPIO_InitStruct);
}

DHT22_RESULT DHT22_Init(TIM_HandleTypeDef* handle) {

	INITBEGIN;
	dht.gpioPin = DHT22_Pin;
	dht.gpioPort = DHT22_GPIO_Port;
	dht.timChannel = TIM_CHANNEL_4;
	dht.timHandle = handle;

	D("&dht: %p",&dht);

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
	HAL_GPIO_WritePin(handle->gpioPort, handle->gpioPin, GPIO_PIN_SET);
	osDelay(2000);
	HAL_GPIO_WritePin(handle->gpioPort, handle->gpioPin, GPIO_PIN_RESET);
	osDelay(2);
	handle->bitPos = -1;
	for (int i = 0; i < 5; i++) {
		handle->bitsRX[i] = 0;
	}

	handle->lastVal=0;
	handle->timHandle->Instance->CR1 = 0;
	DHT22_SetPinIN(handle);
	handle->state = DHT22_RECEIVING;
	DHT22_StartIT();

	return DHT22_OK;
}

void DHT22_InterruptHandler(TIM_HandleTypeDef* htim) {

	uint32_t val = HAL_TIM_ReadCapturedValue(htim,
			dht.timChannel);

	uint32_t freq = HAL_RCC_GetPCLK2Freq()/72;

	uint32_t val2;
	if (val > dht.lastVal)
		val2 = val - dht.lastVal;
	else
		val2 = (20000 + dht.lastVal)-val;

	dht.lastVal = val;

	uint32_t time =  ((uint32_t)val2*10000000)/freq;

	if (dht.bitPos < 0) {
		if (time > 1550 && time < 1650) {
			dht.bitPos = 0;
		}
	} else if (dht.bitPos >= 0 && dht.bitPos < 40) {
		if (time > 700 && time < 1000) {
			dht.bitsRX[dht.bitPos / 8] &= ~(1
					<< (7 - dht.bitPos % 8));
			dht.bitPos++;
		} else if (time > 1200 && time < 1650) {
			dht.bitsRX[dht.bitPos / 8] |= 1 << (7 - dht.bitPos % 8);
			dht.bitPos++;
		} else {
			dht.bitPos = -1;
			HAL_TIM_IC_Stop_IT(htim, dht.timChannel);
			dht.state = DHT22_READY;
		}
	}

	if(dht.bitPos==40){
		dht.bitPos = -1;
		HAL_TIM_IC_Stop_IT(htim, dht.timChannel);
		uint8_t sum = 0;
		for (int i = 0; i < 4; i++) {
			sum += dht.bitsRX[i];
		}
		if (sum == dht.bitsRX[4]) {
			dht.crcErrorFlag = 0;

			int16_t temp10 = 0;
			if ((dht.bitsRX[2] & 0x80) == 0x80) {
				temp10 |= (dht.bitsRX[2] & 0x7F) << 8;
				temp10 |= dht.bitsRX[3];
				temp10 *= -1;
			} else {
				temp10 |= dht.bitsRX[2] << 8;
				temp10 |= dht.bitsRX[3];
			}
			dht.temp = temp10;

			int16_t hum10 = 0;
			if ((dht.bitsRX[0] & 0x80) == 0x80) {
				hum10 |= (dht.bitsRX[0] & 0x7F) << 8;
				hum10 |= dht.bitsRX[1];
				hum10 *= -1;
			} else {
				hum10 |= dht.bitsRX[0] << 8;
				hum10 |= dht.bitsRX[1];
			}
			dht.hum = hum10;
		} else {
			dht.crcErrorFlag = 1;
		}
		dht.state = DHT22_RECEIVED;
	}

}

DHT22_RESULT DHT22_ReadData(DHT22_HandleTypeDef* handle) {

	DHT22_InitiateTransfer(handle);

	osDelay(800);

	D("temp: %d, hum: %d, crc: %d",handle->temp,handle->hum,handle->crcErrorFlag);
	if(handle->crcErrorFlag==1){
		return DHT22_CRC_ERROR;
	}
	if(handle->state!=DHT22_RECEIVED){
		DHT22_StopIT();
		return DHT22_ERROR;
	}else{
		handle->state=DHT22_READY;
	}

	return DHT22_OK;
}


int32_t getDHT22_Temperature(uint8_t sensor) {
	SetSensorChannel(sensor -1);
	DHT22_ReadData(&dht);
	ClearSensorChannel();

	return dht.temp;
}

int32_t getDHT22_Humidity(uint8_t sensor) {
	SetSensorChannel(sensor -1);
	DHT22_ReadData(&dht);
	ClearSensorChannel();

	return dht.hum;
}
