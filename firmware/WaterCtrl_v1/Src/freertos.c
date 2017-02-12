/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  *
  * Copyright (c) 2017 STMicroelectronics International N.V. 
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"

/* USER CODE BEGIN Includes */     

#include "adc.h"
#include "spicomm.h"
#include "motors.h"
#include "sensor.h"
#include "analog.h"
#include "moistureMeasure.h"
#include "relais.h"
#include "dht22.h"
#include "stuff.h"
#include "tim.h"
#include "main.h"

/* USER CODE END Includes */

/* Variables -----------------------------------------------------------------*/
osThreadId defaultTaskHandle;

/* USER CODE BEGIN Variables */

osThreadId AliveTickerHandle;
osThreadId WatchdogHandle;
osThreadId SpiBrokerHandle;
osThreadId MotorHandle;
osThreadId SensorHandle;

/* USER CODE END Variables */

/* Function prototypes -------------------------------------------------------*/
void StartDefaultTask(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

/* Hook prototypes */
void vApplicationStackOverflowHook(xTaskHandle xTask, signed char *pcTaskName);

/* USER CODE BEGIN 4 */
void vApplicationStackOverflowHook(xTaskHandle xTask, signed char *pcTaskName)
{
	E("Stack overflow!");

	SpiBuffer buf;
	buf.d[0] = SPI_ID_ERROR;
	buf.d[1] = ERROR_STACKOVERFLOW;
	SpiSend(&buf);

	/* Run time stack overflow checking is performed if
   configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2. This hook function is
   called if a stack overflow is detected. */
}
/* USER CODE END 4 */

/* Init FreeRTOS */

void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
	// nice greetings
	printf_("\r\n\r\nWaterCtrl version v%u.%u [%s]\r\n", VER_MAJOR, VER_MINOR, __DATE__);
	printf_("System clock: %dMHz\r\n", (uint8_t)(SystemCoreClock / 1000000));
	printf_(":: Booting system ...\r\n");

	// setup SPI
	initSpi();
	initMoistureMeasure(&htim3);
	DHT22_Init(&htim2);
	initMotorControl(&htim2);
	InitSensors();
	initAnalogMeasurement(&hadc1);
	InitRelais();

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
	/* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
	/* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
	/* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityLow, 0, 128);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* USER CODE BEGIN RTOS_THREADS */

	osThreadDef(Sensor, procSensor, osPriorityNormal, 1, 64);
	SensorHandle = osThreadCreate(osThread(Sensor), NULL);

#ifdef USEWATCHDOG
	osThreadDef(Watchdog, procWatchdog, osPriorityAboveNormal, 0, 64);
	WatchdogHandle = osThreadCreate(osThread(Watchdog), NULL);
#endif


	//osThreadDef(AliveTicker, procAliveTicker, osPriorityAboveNormal, 0, 64);
	//AliveTickerHandle = osThreadCreate(osThread(AliveTicker), NULL);

	osThreadDef(SpiBroker, procSpiBroker, osPriorityLow, 1, 64);
	SpiBrokerHandle = osThreadCreate(osThread(SpiBroker), NULL);

	osThreadDef(Motor, procMotor, osPriorityHigh, 1, 64);
	MotorHandle = osThreadCreate(osThread(Motor), NULL);


  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_QUEUES */

	printf_(":: Starting threads ...\r\n");

  /* USER CODE END RTOS_QUEUES */
}

/* StartDefaultTask function */
void StartDefaultTask(void const * argument)
{

  /* USER CODE BEGIN StartDefaultTask */
	uint32_t counter = 0;
	// this is a auto generated dummy, so suspend it
	//osThreadSuspend(NULL);
	/* Infinite loop */

	for(;;)
	{
		//SpiBuffer buf;
		//buf.d[0] = SPI_ID_MESSAGE;
		//buf.d[1] = MESSAGE_PING;
#if 0
		int16_t temp = getDHT22_Temperature(1);
		D("frq: %d",temp);

		int16_t value = getAnalogValue(2,0);
		//D("Analaog 1: %d",value);
		//value = getAnalogValue(2,0);
		D("Analaog 2: %d",value);
		value = getAnalogValue(3,0);
		D("Analaog 3: %d",value);
//		value = getAnalogValue(4,0);
//		D("Analaog 4: %d",value);
		value = getSensorFrequency(4)/(uint16_t)1000;
		D("FRequenz: %d khz",value);
#endif
		osDelay(1000);
#if (0)
		MotorCmd* cmd = (MotorCmd*)osPoolAlloc(motorCtrlPool);
		cmd->motor = 1;
		cmd->time = 2;
		cmd->speed = 100;
		osMessagePut(motorCtrlQueue, (uint32_t)cmd, 0);
		osPoolFree(motorCtrlPool,cmd);
#endif
		//	stSensorCmd *sens_cmd = (stSensorCmd*)osPoolAlloc(sensorPool);
		//	sens_cmd->sensor = 3;
		//	osMessagePut(sensorQueue,(uint32_t)sens_cmd,0);
		//	osPoolFree(sensorPool,sens_cmd);

		HAL_GPIO_TogglePin(LED_ERR_GPIO_Port,LED_ERR_Pin);
		counter++;
		//	D("Default task: %d",counter);

	}
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Application */

/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
