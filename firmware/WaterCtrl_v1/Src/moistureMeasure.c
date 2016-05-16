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

#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_gpio.h"

#include <cmsis_os.h>

#include <spicomm.h>
#include <moistureMeasure.h>
#include <broker.h>



#define SYSNAME "Sensor"

/* Private typedef -----------------------------------------------------------*/
osMessageQId sensorQueue;

osMessageQDef(_sensorQueue, 16, stSensorCmd*);

osPoolId  sensorPool;
osPoolDef(_sensorPool, 32, stSensorCmd*);
/* Private define ------------------------------------------------------------*/
#define MOISTURE_SENS_PIN_A0 GPIO_PIN_1
#define MOISTURE_SENS_PIN_A1 GPIO_PIN_10
#define MOISTURE_SENS_PIN_A2 GPIO_PIN_11

#define MOISTURE_SENS_PIN_ENABLE GPIO_PIN_15

#define MOISTURE_MEASURE_CHANNEL_MAX 0x04u
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


uint_fast32_t frequency[MOISTURE_MEASURE_CHANNEL_MAX+1];

uint16_t stateRegister = MOISTURE_MEASURE_STATE_INACTIVE;
uint activeChannel = MOISTURE_MEASURE_CHANNEL0_ACTIVE;

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
  * @param  ptrTimerRef Timer to use
  * @param  channel Channel to use
  * @retval return value
  */
int startSensorCapture(int channel);

//static void moiBrokerMessage(char * buf, uint8_t length);
void ClearSensorSelectPin();
void SetSensorSelectPin();
void moiSetChannel(int);

/* Global data */
TIM_HandleTypeDef * ptrTimer3Ref;
uint32_t TimerChannel = TIM_CHANNEL_3;

uint32_t getSensorFrequency(int sensor)
{
	uint32_t channel = 0;
	channel = sensor - 1 ;
    if ((MOISTURE_MEASURE_CHANNEL0_ACTIVE <= channel) \
            &&(MOISTURE_MEASURE_CHANNEL_MAX >= channel))
        return frequency[channel];
    else
        return 0;
}

int initMoistureMeasure(TIM_HandleTypeDef * ptr) {
	INITBEGIN;
    I( "init moisture measure system");
    stateRegister |= MOISTURE_MEASURE_STATE_ACTIVE;
	// init queues
	sensorQueue = osMessageCreate(osMessageQ(_sensorQueue), NULL);

	// init data pools
	sensorPool = osPoolCreate(osPool(_sensorPool));
    ptrTimer3Ref = ptr;
    memset(frequency,0,sizeof(frequency));

    startSensorCapture(activeChannel);
    INITEND;
    return 0;
}

int startSensorCapture(int Sensor) {
    StopTimer(ptrTimer3Ref,TimerChannel);

    // select input
    moiSetChannel(Sensor);

    StartTimer(ptrTimer3Ref,TimerChannel);
    stateRegister |= MOISTURE_MEASURE_STATE_ACTIVE;

    return Sensor;
}

void printMoisture() {
	D( "Measured channel 1: %d Hz",
        (int)frequency[MOISTURE_MEASURE_CHANNEL0_ACTIVE]);
    D( "Measured channel 2: %d Hz",
        (int)frequency[MOISTURE_MEASURE_CHANNEL1_ACTIVE]);
    D( "Measured channel 3: %d Hz",
        (int)frequency[MOISTURE_MEASURE_CHANNEL2_ACTIVE]);
    D( "Measured channel 4: %d Hz",
        (int)frequency[MOISTURE_MEASURE_CHANNEL3_ACTIVE]);
    D( "Measured channel 5: %d Hz",
        (int)frequency[MOISTURE_MEASURE_CHANNEL4_ACTIVE]);
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

void SetSensorSelectPin() {
    HAL_GPIO_WritePin(GPIOB,MOISTURE_SENS_PIN_ENABLE,GPIO_PIN_SET);
}

void ClearSensorSelectPin() {
    HAL_GPIO_WritePin(GPIOB,MOISTURE_SENS_PIN_ENABLE,GPIO_PIN_RESET);
}

void moiSetChannel(int channel)
{
    volatile int16_t tSensorSelect = 0;

    ClearSensorSelectPin();
    resetFrequencyOfChannel();
    tSensorSelect = MOISTURE_SENS_PIN_A0|MOISTURE_SENS_PIN_A1|MOISTURE_SENS_PIN_A2;

    switch (channel) {
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
    HAL_GPIO_WritePin(GPIOB,(MOISTURE_SENS_PIN_A0|MOISTURE_SENS_PIN_A1|MOISTURE_SENS_PIN_A2),GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB,tSensorSelect,GPIO_PIN_SET);
    SetSensorSelectPin();
}

void MoistureTask() {
    if (stateRegister == MOISTURE_MEASURE_STATE_ACTIVE) {
        frequency[activeChannel] = getFrequencyOfChannel();
        activeChannel++;
        if (activeChannel > MOISTURE_MEASURE_CHANNEL_MAX)
            activeChannel = MOISTURE_MEASURE_CHANNEL0_ACTIVE;
        startSensorCapture(activeChannel);
    } else {
        //E( "MOI inactive state");
    }
}

void procSensor(void const * argument) {
	PROCRUNNING;
	osEvent event;
	bool run = true;
	do {
		event = osMessageGet(sensorQueue,1000);
		switch(event.status) {
		case osEventMessage: {
			D("message: 0x%02x",event.value.p);
			SpiBuffer spi;
			memset(&spi,0,sizeof(spi));
			stSensorCmd *cmd = event.value.p;
			cmd->value = getSensorFrequency(cmd->sensor)/(uint16_t)1000;
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

/**
  * @}
  */


