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
#include <broker.h>

#include "sensor.h"
/* Private typedef -----------------------------------------------------------*/
osMessageQId sensorQueue;
osMessageQDef(_sensorQueue, 16, stSensorCmd*);

osPoolId  sensorPool;
osPoolDef(_sensorPool, 32, stSensorCmd*);

void procSensor(void const * argument) {
	PROCRUNNING;

	osEvent event;
	bool run = true;

	// init queues
	sensorQueue = osMessageCreate(osMessageQ(_sensorQueue), NULL);

	// init data pools
	sensorPool = osPoolCreate(osPool(_sensorPool));

	do {
		event = osMessageGet(sensorQueue,1000);
		switch(event.status) {
		case osEventMessage: {
			D("message: 0x%02x",event.value.p);
			SpiBuffer spi;
			memset(&spi,0,sizeof(spi));
			stSensorCmd *cmd = event.value.p;
			cmd->value = getMoinstureFrequency(cmd->sensor)/(uint16_t)1000;
			spi.d[3] = (uint8_t)(cmd->value);
			spi.d[2] = (uint8_t)(cmd->value>>8);
			spi.d[1] = cmd->sensor;
			spi.d[0] = BRK_MSG_SPI_ID_SENS_VALUE_RSP;
			D("RESP: sens: 0x%02x value: %d",cmd->sensor,cmd->value);
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
