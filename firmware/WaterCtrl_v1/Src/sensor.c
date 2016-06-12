/*
 * sensor.c
 *
 *  Created on: Jun 11, 2016
 *      Author: micha
 */

#include "stdio.h"
#include "stdlib.h"
#include "stdint.h"
#include "stdbool.h"

#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_gpio.h"

#include <cmsis_os.h>

#include <spicomm.h>
#include <moistureMeasure.h>
#include <dht22.h>

#include "sensor.h"

/* Private defines */
#define SYSNAME __FILE__
/* Private typedef -----------------------------------------------------------*/
osMessageQId sensorQueue;
osMessageQDef(_sensorQueue, 16, stSensorCmd*);

osPoolId  sensorPool;
osPoolDef(_sensorPool, 32, stSensorCmd*);

eSensorType sensorConfig[SEN_CHANNEL7_ACTIVE+1];

/* Private functions */
void ClearSensorSelectPin();
void SetSensorSelectPin();

void procSensor(void const * argument) {
	PROCRUNNING;

	osEvent event;
	bool run = true;

	do {
		event = osMessageGet(sensorQueue,1000);
		switch(event.status) {
		case osEventMessage: {
			SpiBuffer spi;
			stSensorCmd *cmd = event.value.p;

			D("message: 0x%02x",cmd);
			memset(&spi,0,sizeof(spi));

			switch(cmd->cmd) {
			case SPI_ID_SEN_MEA_REQ: {
				switch(sensorConfig[cmd->sensor-1]){
				case SENS_MOISTURE:
					cmd->value1 = getSensorFrequency(cmd->sensor)/(int16_t)1000;
					break;
				case SENS_DHT22:
					cmd->value1 = getDHT22_Temperature();
					cmd->value2 = getDHT22_Humidity();
					break;
				default:
					break;
				}
				spi.d[5] = (uint8_t)(cmd->value2);
				spi.d[4] = (uint8_t)(cmd->value2>>8);
				spi.d[3] = (uint8_t)(cmd->value1);
				spi.d[2] = (uint8_t)(cmd->value1>>8);
				spi.d[0] = SPI_ID_SEN_MEA_VALUE;
				break;
			}
			case SPI_ID_SEN_TYP_REQ:
				sensorConfig[cmd->sensor] = *(eSensorType*)(cmd+3);
				spi.d[0] = SPI_ID_SEN_TYP_RES;
				break;
			default:
				break;
			}
			spi.d[1] = cmd->sensor;
			D("RESP: sens: 0x%02x cmd: 0x%02x value1: %d value2: %d",cmd->sensor,cmd->cmd,cmd->value1,cmd->value2);
			SpiSend(&spi);
			//printMoisture();
			osPoolFree(sensorPool, cmd);
			break;
		}
		case osEventTimeout: {
			MoistureTask();
			break;
		}
		case osErrorOS:
			E("osErrorOS");
			run = false;
			break;
		default:
			//MoistureTask();
			break;
		}
	}while(run);
}
void SetSensorSelectPin() {
	HAL_GPIO_WritePin(SEN_ENABLE_GPIO_Port,SEN_ENABLE_Pin,GPIO_PIN_SET);
}

void ClearSensorSelectPin() {
	HAL_GPIO_WritePin(SEN_ENABLE_GPIO_Port,SEN_ENABLE_Pin,GPIO_PIN_RESET);
}
void SetSensorChannel(int channel){

	volatile int16_t tSensorSelect = 0;

	ClearSensorSelectPin();
	tSensorSelect = SEN_A0_Pin|SEN_A1_Pin|SEN_A1_Pin;

	switch (channel) {
	case SEN_CHANNEL0_ACTIVE:
		tSensorSelect &= ~(SEN_A0_Pin|SEN_A1_Pin|SEN_A2_Pin);
		break;
	case SEN_CHANNEL1_ACTIVE:
		tSensorSelect &= ~(SEN_A1_Pin|SEN_A2_Pin);
		break;
	case SEN_CHANNEL2_ACTIVE:
		tSensorSelect &= ~(SEN_A0_Pin|SEN_A2_Pin);
		break;
	case SEN_CHANNEL3_ACTIVE:
		tSensorSelect &= ~(SEN_A2_Pin);
		break;
	case SEN_CHANNEL4_ACTIVE:
		tSensorSelect &= ~(SEN_A0_Pin|SEN_A1_Pin);
		break;
	case SEN_CHANNEL5_ACTIVE:
		tSensorSelect &= ~(SEN_A1_Pin);
		break;
	case SEN_CHANNEL6_ACTIVE:
		tSensorSelect &= ~(SEN_A0_Pin);
		break;
	case SEN_CHANNEL7_ACTIVE:
		break;
	default:
		break;
	}
	tSensorSelect &= (SEN_A0_Pin|SEN_A1_Pin|SEN_A2_Pin);
	HAL_GPIO_WritePin(GPIOB,(SEN_A0_Pin|SEN_A1_Pin|SEN_A2_Pin),GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB,tSensorSelect,GPIO_PIN_SET);
	SetSensorSelectPin();
}

void InitSensors() {
	INITBEGIN;

	memset(sensorConfig,0,sizeof(sensorConfig));
	// init queues
	sensorQueue = osMessageCreate(osMessageQ(_sensorQueue), NULL);

	// init data pools
	sensorPool = osPoolCreate(osPool(_sensorPool));
	INITEND;
}
