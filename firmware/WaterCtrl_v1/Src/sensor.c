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
#include <analog.h>
#include <main.h>


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
eSensorError SetSensorSelectPin();
eSensorError getSensor(stSensorCmd* cmd);
eSensorError setSensorType(stSensorCmd* cmd);

void procSensor(void const * argument) {
	PROCRUNNING;

	osEvent event;
	bool run = true;

	sensorConfig[0] = SENS_DHT22;
	sensorConfig[1] = SENS_ANALOG_PA2;
	sensorConfig[2] = SENS_ANALOG_PA2;
	sensorConfig[3] = SENS_MOISTURE;
	sensorConfig[4] = SENS_MOISTURE;

	do {
		event = osMessageGet(sensorQueue,1000);
		switch(event.status) {
		case osEventMessage: {
			SpiBuffer spi;
			stSensorCmd *cmd = event.value.p;

			D("message: 0x%02x",cmd);
			memset(&spi,0,sizeof(spi));

			switch(cmd->cmd) {
			case SPI_ID_SEN_MEA_REQ:
				if ( SENS_ERR_NO == getSensor(cmd) )
					spi.d[0] = SPI_ID_SEN_MEA_VALUE;
				else
					spi.d[0] = SPI_ID_ERROR;
				break;
			case SPI_ID_SEN_TYP_REQ:
				if (SENS_ERR_NO == setSensorType(cmd))
					spi.d[0] = SPI_ID_SEN_TYP_RES;
				else
					spi.d[0] = SPI_ID_ERROR;
				break;
			default:
				break;
			}
			spi.d[5] = (uint8_t)(cmd->value2);
			spi.d[4] = (uint8_t)(cmd->value2>>8);
			spi.d[3] = (uint8_t)(cmd->value1);
			spi.d[2] = (uint8_t)(cmd->value1>>8);
			spi.d[1] = cmd->sensor;
			D("RESP: sens: 0x%02x cmd: 0x%02x value1: %d value2: %d",cmd->sensor,cmd->cmd,cmd->value1,cmd->value2);
			SpiSend(&spi);
			osPoolFree(sensorPool, event.value.p);
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
eSensorError SetSensorSelectPin() {
	eSensorError ret = SENS_ERR_NO;
	HAL_GPIO_WritePin(SEN_ENABLE_GPIO_Port,SEN_ENABLE_Pin,GPIO_PIN_SET);

	return ret;
}

void ClearSensorSelectPin() {
	HAL_GPIO_WritePin(SEN_ENABLE_GPIO_Port,SEN_ENABLE_Pin,GPIO_PIN_RESET);
}


eSensorError SetSensorChannel(int channel){

	eSensorError ret = SENS_ERR_NO;
	if ( (SEN_CHANNEL0_ACTIVE <= channel ) && (SEN_CHANNEL4_ACTIVE >= channel))
	{
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
		if ( SENS_ERR_NO != (ret = SetSensorSelectPin()) )
		{
			W("Failed set select pin");
		}
		else
		{
			D("Successfully set sensor channel %d",channel);
		}
	}
	else
	{
		ret = SENS_ERR_BAD_CHAN;
	}
	return ret;
}

void ClearSensorChannel()
{

	ClearSensorSelectPin();
}

eSensorError getSensor(stSensorCmd* cmd)
{
	eSensorError ret = SENS_ERR_NO;
	int channel = cmd->sensor-1;
	if ( (SEN_CHANNEL0_ACTIVE <= channel) && (SEN_CHANNEL4_ACTIVE >= channel))
	{
		D("get measuring of channel %d (type: %d)",cmd->sensor,sensorConfig[channel]);
		switch(sensorConfig[channel]){
		case SENS_MOISTURE:
			cmd->value1 = getSensorFrequency(cmd->sensor)/(int16_t)1000;
			break;
		case SENS_DHT22:
			cmd->value1 = getDHT22_Humidity(cmd->sensor);
			break;
		case SENS_DHT22_TEMP:
			cmd->value1 = getDHT22_Temperature(cmd->sensor);
			break;
		case SENS_ANALOG_PA3:
			cmd->value1 = getAnalogValue(cmd->sensor,1);
			//cmd->value1 = getAnalogValue(cmd->sensor,1);
			break;
		case SENS_ANALOG_PA2:
			cmd->value1 = getAnalogValue(cmd->sensor,0);
			break;
		default:
			break;
		}
	}
	else
	{
		ret = SENS_ERR_BAD_CHAN;
	}

	// limit sensor value to range of 0.0 - 100.0%
	if ((cmd->value1 < 0) && (cmd->value1 > 1000))
	{
		W("bad value1 %d",cmd->value1);
		ret = SENS_ERR_BAD_RETURN;
	}
	if ((cmd->value2 < 0) && (cmd->value2 > 1000))
	{
		W("bad value2 %d",cmd->value2);
		ret = SENS_ERR_BAD_RETURN;
	}
	return ret;
}

eSensorError setSensorType(stSensorCmd* cmd)
{
	eSensorError ret = SENS_ERR_NO;
	int channel = cmd->value1 -1;
	if ((SEN_CHANNEL0_ACTIVE <= channel) && (SEN_CHANNEL4_ACTIVE >= channel))
	{
		if ((SENS_MOISTURE<=cmd->value1) && (SENS_ANALOG_PA3 >= cmd->value1))
			sensorConfig[channel] = cmd->value1;
		else
			ret = SENS_ERR_BAD_PARAM;
	}
	else
		ret = SENS_ERR_BAD_CHAN;

	return ret;
}

void InitSensors()
{
	INITBEGIN;

	memset(sensorConfig,0,sizeof(sensorConfig));
	// init queues
	sensorQueue = osMessageCreate(osMessageQ(_sensorQueue), NULL);

	// init data pools
	sensorPool = osPoolCreate(osPool(_sensorPool));
	INITEND;
}
