/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  *
  * COPYRIGHT(c) 2016 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"

/* USER CODE BEGIN Includes */     


#include "spicomm.h"
#include "motors.h"
#include "moistureMeasure.h"
#include "stuff.h"
#include "tim.h"
#include "mxconstants.h"

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
    initMotorControl(&htim2);



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
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 1, 32);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  osThreadDef(Sensor, procSensor, osPriorityNormal, 1, 64);
  SensorHandle = osThreadCreate(osThread(Sensor), NULL);

  /* USER CODE BEGIN RTOS_THREADS */


#ifdef USEWATCHDOG
  osThreadDef(Watchdog, procWatchdog, osPriorityAboveNormal, 0, 64);
  WatchdogHandle = osThreadCreate(osThread(Watchdog), NULL);
#endif


  osThreadDef(AliveTicker, procAliveTicker, osPriorityAboveNormal, 0, 64);
  AliveTickerHandle = osThreadCreate(osThread(AliveTicker), NULL);

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
	PROCRUNNING;
  /* USER CODE BEGIN StartDefaultTask */
	uint32_t counter = 0;
	// this is a auto generated dummy, so suspend it
	//osThreadSuspend(NULL);

  /* Infinite loop */
  for(;;)
  {
	SpiBuffer buf;
	buf.d[0] = SPI_ID_MESSAGE;
	buf.d[1] = MESSAGE_PING;


    osDelay(3000);
	//SpiSend(&buf);

	//osDelay(3000);
//	MotorCmd* cmd = (MotorCmd*)osPoolAlloc(motorCtrlPool);
//	cmd->motor = 1;
//	cmd->time = 2;
//	cmd->speed = 50;
//	osMessagePut(motorCtrlQueue, (uint32_t)cmd, 0);
//	osPoolFree(motorCtrlPool,cmd);

//	stSensorCmd *sens_cmd = (stSensorCmd*)osPoolAlloc(sensorPool);
//	sens_cmd->sensor = 3;
//	osMessagePut(sensorQueue,(uint32_t)sens_cmd,0);
//	osPoolFree(sensorPool,sens_cmd);

	counter++;
	D("Default task: %d",counter);

  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Application */
     
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
