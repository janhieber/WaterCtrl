/*
 * moistureMeasure.c
 *
 *  Created on: 24.04.2015
 *      Author: jan
 */


#include <stm32f0xx_conf.h>

#include <moistureMeasure.h>
#include <helpers.h>

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
  * @brief  Configure the TIM1 Pins.
  * @param  None
  * @retval None
  */
void TIM_MeasureInit(void);

int startSensorCapture(int channel);

uint_fast64_t getMoisture(int channel)
{
	return getFrequencyOfChannel(channel);
}

int initMoistureMeasure(void) {

	stateRegister |= MOISTURE_MEASURE_STATE_ACTIVE;

	startSensorCapture(0);

	return 0;
}

int startSensorCapture(int Sensor)
{
	// select input

	TIM_MeasureInit();

	Delay(500);

	return Sensor;

}

void printMoisture()
{
	volatile uint64_t value =0;

	for (int channel=0;channel<1;channel++)
	{
		value = getMoisture(channel);
		printf("Measured channel %d: %d Hz\t\t", channel, value);
		printf("freq: %d\n",value);
	}
}

void TIM_MeasureInit(void)
{
  TIM_ICInitTypeDef  TIM_ICInitSt;
  GPIO_InitTypeDef GPIO_InitSt;

  /* TIM1 clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

  /* TIM1 channel 2 pin (PE.11) configuration */
  GPIO_InitSt.GPIO_Pin =  GPIO_Pin_9;
  GPIO_InitSt.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitSt.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitSt.GPIO_OType = GPIO_OType_PP;
  GPIO_InitSt.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOA, &GPIO_InitSt);

  /* Connect TIM pins to AF2 */
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_2);

  /* TIM1 configuration: Input Capture mode ---------------------
     The external signal is connected to TIM1 CH2 pin (PA.09)
     The Rising edge is used as active edge,
     The TIM1 CCR2 is used to compute the frequency value
  ------------------------------------------------------------ */

  TIM_ICInitSt.TIM_Channel = TIM_Channel_2;
  TIM_ICInitSt.TIM_ICPolarity = TIM_ICPolarity_Rising;
  TIM_ICInitSt.TIM_ICSelection = TIM_ICSelection_DirectTI;
  TIM_ICInitSt.TIM_ICPrescaler = TIM_ICPSC_DIV8;
  TIM_ICInitSt.TIM_ICFilter = 0x03;

  TIM_ICInit(TIM1, &TIM_ICInitSt);

  /* TIM enable counter */
  TIM_Cmd(TIM1, ENABLE);

  /* Enable the CC2 Interrupt Request */
  TIM_ITConfig(TIM1, TIM_IT_CC2, ENABLE);
}


void TIM_EnableMeasureInterrupt()
{
	  NVIC_InitTypeDef NVIC_InitSt;
/* Enable the TIM1 global Interrupt */
NVIC_InitSt.NVIC_IRQChannel = TIM1_CC_IRQn;
NVIC_InitSt.NVIC_IRQChannelPriority = 0;
NVIC_InitSt.NVIC_IRQChannelCmd = ENABLE;
NVIC_Init(&NVIC_InitSt);

return;
}
