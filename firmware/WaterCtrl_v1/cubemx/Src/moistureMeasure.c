/* ---------------------------------------------------------------------------
** This software is in the public domain, furnished "as is", without technical
** support, and with no warranty, express or implied, as to its usefulness for
** any purpose.
**
** moistureMeasure.c
** *see header file*
**
** Author: dropedout
** -------------------------------------------------------------------------*/

#include "stm32f1xx_hal.h"

#include <moistureMeasure.h>

#include "stdio.h"
#include "stdlib.h"

/**
 * @brief Measure frequency connected to TIM inputs
 *
 * 	Measure and  calculate floating mean of the frequency via interrupt.
 *
 * 	@retVal Global frequency in ulong
 *
 *
 * 	ToDo:
 * 	Make input pin configurable. We need to measure 8 sensor inputs
 * 	Find algorithm which less stress the output
 * 	- lower measurement period time
 * 	- combine capture inputs to gain more precision/range
 * 	-
 * 	Multiplex inputs
 * 	- GPIO for input selection
 * 	- disable capture while inputs switch
 *
 * 	Attention:
 * 	Concurrent access to global frequency by interrupt and output loop!
 *
 * 	Questions for implementation:
 * 	1. which frequency range is expected -> prescaler
 * 	2. How to implement self stopping measure interval?
 * 		a. How to stop the measurement between channels
 *
 */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define MOISTURE_MEASURE_CHANNEL_MAX 0x08u
#define MOISTURE_MEASURE_STATE_INACTIVE 0x0000
#define MOISTURE_MEASURE_STATE_ACTIVE 0x0001

#define MOISTURE_MEASURE_CHANNEL0_ACTIVE
#define MOISTURE_MEASURE_CHANNEL1_ACTIVE
#define MOISTURE_MEASURE_CHANNEL2_ACTIVE
#define MOISTURE_MEASURE_CHANNEL3_ACTIVE
#define MOISTURE_MEASURE_CHANNEL4_ACTIVE
#define MOISTURE_MEASURE_CHANNEL5_ACTIVE
#define MOISTURE_MEASURE_CHANNEL6_ACTIVE
#define MOISTURE_MEASURE_CHANNEL7_ACTIVE
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/


uint64_t frequency[MOISTURE_MEASURE_CHANNEL_MAX];

uint16_t stateRegister = MOISTURE_MEASURE_STATE_INACTIVE;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

extern uint_fast64_t getFrequencyOfChannel();


/**
  * @brief  Starts Timer and enables interrupt.
  * @param  ptrTimerRef Timer to use
  * @param  channel Channel to use
  * @retval return value
  */
int MeasureInit(TIM_HandleTypeDef * ptrTimerRef,uint32_t channel);

/**
  * @brief  Enable timer interrupt.
  * @param  None
  * @retval None
  */
void EnableMeasureInterrupt();
/**
  * @brief  Disable timer interrupt.
  * @param  None
  * @retval None
  */
void DisableMeasureInterrupt();

int startSensorCapture(int channel);

/* Global data */
TIM_HandleTypeDef * ptrTimer3Ref;
uint32_t Timer3Channel;

uint_fast64_t getMoisture(int channel)
{
    return getFrequencyOfChannel(channel);
}

int initMoistureMeasure(TIM_HandleTypeDef * ptr) {

    stateRegister |= MOISTURE_MEASURE_STATE_ACTIVE;

    ptrTimer3Ref = ptr;
    startSensorCapture(0);

    return 0;
}

int startSensorCapture(int Sensor)
{
    // select input

    MeasureInit(ptrTimer3Ref,Timer3Channel);

    return Sensor;
}

void printMoisture()
{
    uint value =0;

    for (int sensor=0;sensor<1;sensor++)
    {
        value = getMoisture(sensor);
        printf("Measured channel %d: %d Hz\t\t", sensor, value);
        printf("freq: %x\n",value);
    }
}

int MeasureInit(TIM_HandleTypeDef * ptrTimerRef,uint32_t channel)
{
    int retval = -1;

    /* TIM1 configuration: Input Capture mode ---------------------
     The external signal is connected to TIM1 CH2 pin (PA.09)
     The Rising edge is used as active edge,
     The TIM1 CCR2 is used to compute the frequency value
  ------------------------------------------------------------ */

    /*TIM_ICInitSt.TIM_Channel = TIM_Channel_2;
  TIM_ICInitSt.TIM_ICPolarity = TIM_ICPolarity_Rising;
  TIM_ICInitSt.TIM_ICSelection = TIM_ICSelection_DirectTI;
  TIM_ICInitSt.TIM_ICPrescaler = TIM_ICPSC_DIV8;
  TIM_ICInitSt.TIM_ICFilter = 0x03;

  TIM_ICInit(TIM1, &TIM_ICInitSt);*/

    /* TIM enable counter */
    if(ptrTimerRef)
    {
        if (HAL_OK != (retval = HAL_TIM_IC_Start(ptrTimerRef,channel)))
        {
            printf("FAILED: timer start, erro: %d",retval);
        }

    }

    EnableMeasureInterrupt();

    return retval;
}


void EnableMeasureInterrupt()
{
    HAL_NVIC_ClearPendingIRQ(TIM3_IRQn);
    HAL_NVIC_EnableIRQ(TIM3_IRQn);

    return;
}
void DisableMeasureInterrupt()
{
    HAL_NVIC_ClearPendingIRQ(TIM3_IRQn);
    HAL_NVIC_DisableIRQ(TIM3_IRQn);

    return;
}
