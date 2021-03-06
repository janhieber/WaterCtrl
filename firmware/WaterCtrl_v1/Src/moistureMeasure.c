/** @file moistureMeasure.c
 *  @brief Measure frequency connected to TIM inputs
 *      Measure and  calculate floating mean of the frequency via interrupt.
 *
 *  @todo:
 * 	Make input pin configurable. We need to measure 8 sensor inputs
 * 	Find algorithm which less stress the output
 * 	- lower measurement period time
 * 	- combine capture inputs to gain more precision/range
 *
 * 	Multiplex inputs
 * 	- GPIO for input selection
 * 	- disable capture while inputs switch
 *
 * 	@attention:
 * 	Concurrent access to global frequency by interrupt and output loop!
 *
 * 	Questions for implementation:
 * 	1. which frequency range is expected -> prescaler
 * 	2. How to implement self stopping measure interval?
 * 		a. How to stop the measurement between channels
 *
 *  @author Dropedout
 */

/** @addtogroup MoistureMeasure
 * @{
 */

#include "stdio.h"
#include "stdlib.h"
#include "stdint.h"
#include "stdbool.h"
#include "string.h"

#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_gpio.h"

#include <cmsis_os.h>

#include <moistureMeasure.h>
#include <sensor.h>

#include "main.h"

#define SYSNAME "Moisture"

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

#define MOISTURE_SENS_PIN_ENABLE GPIO_PIN_15

#define MOISTURE_MEASURE_CHANNEL_MAX 0x04u
#define MOISTURE_MEASURE_STATE_INACTIVE 0x0000
#define MOISTURE_MEASURE_STATE_ACTIVE 0x0001


/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/


uint_fast32_t frequency[MOISTURE_MEASURE_CHANNEL_MAX+1];

uint16_t stateRegister = MOISTURE_MEASURE_STATE_INACTIVE;
uint activeChannel = SEN_CHANNEL0_ACTIVE;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/**
 * @brief  Get ISR's data.
 *
 * @see isr_moisture.c
 */
extern uint32_t getFrequencyOfChannel();

/**
 * @brief  Reset ISR's data.
 *
 * @see isr_moisture.c
 */
extern void resetFrequencyOfChannel();


/**
 * @brief  Starts Timer and enables interrupt.
 * @param  ptrTimerRef Timer to use
 * @param  channel Channel to use
 * @retval return value
 */
int StartTimer(TIM_HandleTypeDef * ptrTimerRef,uint32_t channel);

/**
 * @brief  Stop Timer and enables interrupt.
 * @param  ptrTimerRef Timer to use
 * @param  channel Channel to use
 * @retval return value
 */
int StopTimer(TIM_HandleTypeDef * ptrTimerRef,uint32_t channel);

/**
 * @brief  Starts Timer and enables interrupt.
 * @param  channel Channel to use
 * @retval return value
 */
eSensorError startSensorCapture(int channel);

/**
 * @brief  Stop timer and set state to inactive.
 * @retval return value
 */
int stopSensorCapture();

/* Global data */
TIM_HandleTypeDef * ptrTimer3Ref;
uint32_t TimerChannel = TIM_CHANNEL_3;

uint32_t getSensorFrequency(int sensor)
{
	uint32_t channel = sensor - 1 ;
	if (SENS_ERR_NO == startSensorCapture(channel))
	{
		osDelay(1000);
		stopSensorCapture();
		return frequency[channel];
	} else
		return 0;
}

int initMoistureMeasure(TIM_HandleTypeDef * ptr) {
	INITBEGIN;

	stateRegister &= MOISTURE_MEASURE_STATE_INACTIVE;

	ptrTimer3Ref = ptr;
	memset(frequency,0,sizeof(frequency));

	INITEND;
	return 0;
}

eSensorError startSensorCapture(int channel) {
	eSensorError ret = SENS_ERR_NO;

	GPIO_InitTypeDef GPIO_InitStruct;

	StopTimer(ptrTimer3Ref,TimerChannel);

	resetFrequencyOfChannel();

	if (SENS_ERR_NO == (ret = SetSensorChannel(channel)))
	{
		activeChannel = channel;

		GPIO_InitStruct.Pin = FREQ_Pin;
		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
		HAL_GPIO_Init(FREQ_GPIO_Port, &GPIO_InitStruct);

		if (HAL_OK == StartTimer(ptrTimer3Ref,TimerChannel))
		{
			stateRegister |= MOISTURE_MEASURE_STATE_ACTIVE;
		}
		else
		{
			ClearSensorChannel();
			ret = SENS_ERR_HAL;
		}
	}
	return ret;
}

int stopSensorCapture() {

	frequency[activeChannel] = getFrequencyOfChannel();
	StopTimer(ptrTimer3Ref,TimerChannel);
	stateRegister = MOISTURE_MEASURE_STATE_INACTIVE;
	ClearSensorChannel();
	return 0;
}


void printMoisture() {
#if 0
	D( "Measured channel 1: %d Hz",
			(int)frequency[SEN_CHANNEL0_ACTIVE]);
	D( "Measured channel 2: %d Hz",
			(int)frequency[SEN_CHANNEL1_ACTIVE]);
	D( "Measured channel 3: %d Hz",
			(int)frequency[SEN_CHANNEL2_ACTIVE]);
	D( "Measured channel 4: %d Hz",
			(int)frequency[SEN_CHANNEL3_ACTIVE]);
	D( "Measured channel 5: %d Hz",
			(int)frequency[SEN_CHANNEL4_ACTIVE]);
#endif
}

int StartTimer(TIM_HandleTypeDef * ptrTimerRef,uint32_t channel) {
	int retval = -1;
	/* TIM enable counter */
	if(ptrTimerRef) {
		if (HAL_OK != (retval = HAL_TIM_IC_Start_IT(ptrTimerRef, channel))) {
			E("FAILED: timer start, erro: %d",retval);
		}
	}
	return retval;
}
int StopTimer(TIM_HandleTypeDef * ptrTimerRef,uint32_t channel) {
	int retval = -1;
	/* TIM disable counter */
	if(ptrTimerRef) {
		if (HAL_OK != (retval = HAL_TIM_IC_Stop_IT(ptrTimerRef, channel))) {
			E("FAILED: timer stop, erro: %d",retval);
		}
	}
	return retval;
}

void MoistureTask() {

	if (stateRegister == MOISTURE_MEASURE_STATE_ACTIVE) {
		// stop measurement after 1 s
		//stopSensorCapture();
		//        frequency[activeChannel] = getFrequencyOfChannel();
		//        activeChannel++;
		//        if (activeChannel > MOISTURE_MEASURE_CHANNEL_MAX)
		//            activeChannel = MOISTURE_MEASURE_CHANNEL0_ACTIVE;
		//        startSensorCapture(activeChannel);
	} else {
		//D( "MOI inactive state");
	}
}


/**
 * @}
 */


