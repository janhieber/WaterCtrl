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
 */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define MOISTURE_MEASURE_CHANNEL_MAX 0x08u
#define MOISTURE_MEASURE_STATE_INACTIVE 0x0000
#define MOISTURE_MEASURE_STATE_ACTIVE 0x0001
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
void TIM_Config(void);

int startSensorCapture(int channel);

uint_fast64_t getMoisture(int channel)
{
	(void*)channel;
	return 0xFFFFFFFFFFFFFFFFu;
}

int initMoistureMeasure(void) {

	stateRegister |= MOISTURE_MEASURE_STATE_ACTIVE;

	startSensorCapture(0);

	return 0;
}

int startSensorCapture(int Sensor)
{
	// select input

	TIM_Config();

	Delay(500);

	return Sensor;

}


void TIM_Config(void)
{
  TIM_ICInitTypeDef  TIM_ICInitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;

  /* TIM1 clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

  /* TIM1 channel 2 pin (PE.11) configuration */
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /* Connect TIM pins to AF2 */
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_2);

  /* TIM1 configuration: Input Capture mode ---------------------
     The external signal is connected to TIM1 CH2 pin (PA.09)
     The Rising edge is used as active edge,
     The TIM1 CCR2 is used to compute the frequency value
  ------------------------------------------------------------ */

  TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
  TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
  TIM_ICInitStructure.TIM_ICFilter = 0x0;

  TIM_ICInit(TIM1, &TIM_ICInitStructure);

  /* TIM enable counter */
  TIM_Cmd(TIM1, ENABLE);

  /* Enable the CC2 Interrupt Request */
  TIM_ITConfig(TIM1, TIM_IT_CC2, ENABLE);

  /* Enable the TIM1 global Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = TIM1_CC_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

}


