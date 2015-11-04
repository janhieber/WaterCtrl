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

#include "stm32f1xx_hal.h"

#include <moistureMeasure.h>
#include "stm32f1xx_hal_gpio.h"
#include "stdio.h"
#include "stdlib.h"



/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define MOISTURE_SENS_PIN_A0 GPIO_PIN_1
#define MOISTURE_SENS_PORT_A0 GPIOB
#define MOISTURE_SENS_PIN_A1 GPIO_PIN_10
#define MOISTURE_SENS_PORT_A1 GPIOB
#define MOISTURE_SENS_PIN_A2 GPIO_PIN_11
#define MOISTURE_SENS_PORT_A2 GPIOB

#define MOISTURE_SENS_PIN_ENABLE GPIO_PIN_15
#define MOISTURE_SENS_PORT_ENABLE GPIOB

#define MOISTURE_MEASURE_CHANNEL_MAX 0x08u
#define MOISTURE_MEASURE_STATE_INACTIVE 0x0000
#define MOISTURE_MEASURE_STATE_ACTIVE 0x0001

#define MOISTURE_MEASURE_CHANNEL0_ACTIVE 0
#define MOISTURE_MEASURE_CHANNEL1_ACTIVE 1
#define MOISTURE_MEASURE_CHANNEL2_ACTIVE 2
#define MOISTURE_MEASURE_CHANNEL3_ACTIVE 3
#define MOISTURE_MEASURE_CHANNEL4_ACTIVE 4
#define MOISTURE_MEASURE_CHANNEL5_ACTIVE 5
#define MOISTURE_MEASURE_CHANNEL6_ACTIVE 6
#define MOISTURE_MEASURE_CHANNEL7_ACTIVE 7
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/


uint64_t frequency[MOISTURE_MEASURE_CHANNEL_MAX];

uint16_t stateRegister = MOISTURE_MEASURE_STATE_INACTIVE;
uint activeChannel = 0;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Get ISR's data.
  *
  * @see isr_moisture.c
  */
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
  */
void EnableMeasureInterrupt();

/**
  * @brief  Disable timer interrupt.
  */
void DisableMeasureInterrupt();

/**
  * @brief  Starts Timer and enables interrupt.
  * @param  ptrTimerRef Timer to use
  * @param  channel Channel to use
  * @retval return value
  */
int startSensorCapture(int channel);

void moiDisableSensor();
void moiEnableSensor();
void moiSetChannel(int);

/* Global data */
TIM_HandleTypeDef * ptrTimer3Ref;
uint32_t Timer3Channel;

uint_fast64_t getMoisture()
{
    return getFrequencyOfChannel();
}

int initMoistureMeasure(TIM_HandleTypeDef * ptr) {
    Log(LogInfo, "init moisture measure system");

    stateRegister |= MOISTURE_MEASURE_STATE_ACTIVE;

    ptrTimer3Ref = ptr;
    startSensorCapture(activeChannel);

    return 0;
}

int startSensorCapture(int Sensor)
{

    MeasureInit(ptrTimer3Ref,Timer3Channel);

    // select input
    moiSetChannel(Sensor);

    return Sensor;
}

void printMoisture()
{
    char tmpbuf[40] = {0,};
    sprintf(tmpbuf, "Measured channel 1: %llu Hz",  frequency[MOISTURE_MEASURE_CHANNEL0_ACTIVE]);
    Log(LogInfo, tmpbuf);
    sprintf(tmpbuf, "Measured channel 2: %llu Hz",  frequency[MOISTURE_MEASURE_CHANNEL1_ACTIVE]);
    Log(LogInfo, tmpbuf);
    sprintf(tmpbuf, "Measured channel 3: %llu Hz",  frequency[MOISTURE_MEASURE_CHANNEL2_ACTIVE]);
    Log(LogInfo, tmpbuf);
    sprintf(tmpbuf, "Measured channel 4: %llu Hz",  frequency[MOISTURE_MEASURE_CHANNEL3_ACTIVE]);
    Log(LogInfo, tmpbuf);
    sprintf(tmpbuf, "Measured channel 5: %llu Hz",  frequency[MOISTURE_MEASURE_CHANNEL4_ACTIVE]);
    Log(LogInfo, tmpbuf);
    sprintf(tmpbuf, "Measured channel 6: %llu Hz",  frequency[MOISTURE_MEASURE_CHANNEL5_ACTIVE]);
    Log(LogInfo, tmpbuf);
    sprintf(tmpbuf, "Measured channel 7: %llu Hz",  frequency[MOISTURE_MEASURE_CHANNEL6_ACTIVE]);
    Log(LogInfo, tmpbuf);
    sprintf(tmpbuf, "Measured channel 8: %llu Hz",  frequency[MOISTURE_MEASURE_CHANNEL7_ACTIVE]);
    Log(LogInfo, tmpbuf);

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
            //printf("FAILED: timer start, erro: %d",retval);
            Log(LogError, "failed to start timer!");
        }

    }

    EnableMeasureInterrupt();

    return retval;
}
void moiEnableSensor()
{
    HAL_GPIO_WritePin(GPIOB,MOISTURE_SENS_PIN_ENABLE,GPIO_PIN_SET);
}
void moiDisableSensor()
{
    HAL_GPIO_WritePin(GPIOB,MOISTURE_SENS_PIN_ENABLE,GPIO_PIN_RESET);
}

void moiSetChannel(int channel)
{
    int tSensorSelect;
    moiDisableSensor();
    DisableMeasureInterrupt();
    tSensorSelect = MOISTURE_SENS_PIN_A0|MOISTURE_SENS_PIN_A1|MOISTURE_SENS_PIN_A2;
    HAL_GPIO_WritePin(GPIOB,tSensorSelect,GPIO_PIN_RESET);
    switch (channel)
    {
    case MOISTURE_MEASURE_CHANNEL0_ACTIVE:
        tSensorSelect &= ~(MOISTURE_SENS_PIN_A0|MOISTURE_SENS_PIN_A1|MOISTURE_SENS_PIN_A2);
        break;
    case MOISTURE_MEASURE_CHANNEL1_ACTIVE:
        tSensorSelect &= ~(MOISTURE_SENS_PIN_A1|MOISTURE_SENS_PIN_A2);
        break;
    case MOISTURE_MEASURE_CHANNEL2_ACTIVE:
        tSensorSelect &= ~(MOISTURE_SENS_PIN_A0|MOISTURE_SENS_PIN_A2);
        break;
    case MOISTURE_MEASURE_CHANNEL3_ACTIVE:
        tSensorSelect &= ~(MOISTURE_SENS_PIN_A2);
        break;
    case MOISTURE_MEASURE_CHANNEL4_ACTIVE:
        tSensorSelect &= ~(MOISTURE_SENS_PIN_A0|MOISTURE_SENS_PIN_A1);
        break;
    case MOISTURE_MEASURE_CHANNEL5_ACTIVE:
        tSensorSelect &= ~(MOISTURE_SENS_PIN_A1);
        break;
    case MOISTURE_MEASURE_CHANNEL6_ACTIVE:
        tSensorSelect &= ~(MOISTURE_SENS_PIN_A0);
        break;
    case MOISTURE_MEASURE_CHANNEL7_ACTIVE:

        break;
    default:
        tSensorSelect &= ~(MOISTURE_SENS_PIN_A0|MOISTURE_SENS_PIN_A1|MOISTURE_SENS_PIN_A2);

        break;
    }
    tSensorSelect &= (MOISTURE_SENS_PIN_A0|MOISTURE_SENS_PIN_A1|MOISTURE_SENS_PIN_A2);
    HAL_GPIO_WritePin(GPIOB,tSensorSelect,GPIO_PIN_SET);
    moiEnableSensor();
    EnableMeasureInterrupt();
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
void MoistureTask()
{
    if (stateRegister == MOISTURE_MEASURE_STATE_ACTIVE)
    {
        frequency[activeChannel] = getMoisture();
        activeChannel++;
        if (activeChannel >= MOISTURE_MEASURE_CHANNEL_MAX)
            activeChannel = MOISTURE_MEASURE_CHANNEL0_ACTIVE;
        moiSetChannel(activeChannel);
    }
    else
    {
        LogUart(LogError, "MOI inactive state");
    }
}

/**
  * @}
  */


