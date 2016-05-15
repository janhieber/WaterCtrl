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

osMessageQId sensorCtrlQueue;
osMessageQDef(sensorCtrlQueue, 16, stSensorCmd);

osPoolId  sensorCtrlPool;
osPoolDef(sensorCtrlPool, 32, stSensorCmd);

/* Private typedef -----------------------------------------------------------*/
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


volatile uint_fast32_t frequency[MOISTURE_MEASURE_CHANNEL_MAX+1];

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
int MeasureInit(TIM_HandleTypeDef * ptrTimerRef,uint32_t channel);

/**
  * @brief  Starts Timer and enables interrupt.
  * @param  ptrTimerRef Timer to use
  * @param  channel Channel to use
  * @retval return value
  */
int startSensorCapture(int channel);

//static void moiBrokerMessage(char * buf, uint8_t length);
void moiDisableSensor();
void moiEnableSensor();
void moiSetChannel(int);

/* Global data */
TIM_HandleTypeDef * ptrTimer3Ref;
uint32_t TimerChannel = TIM_CHANNEL_3;

uint32_t getSensorFrequency(int sensor)
{
    if ((MOISTURE_MEASURE_CHANNEL0_ACTIVE <= sensor) \
            &&(MOISTURE_MEASURE_CHANNEL_MAX >= sensor))
        return frequency[sensor];
    else
        return 0;
}

int initMoistureMeasure(TIM_HandleTypeDef * ptr) {
    I( "init moisture measure system");

    stateRegister |= MOISTURE_MEASURE_STATE_ACTIVE;

    ptrTimer3Ref = ptr;

    memset(&frequency,0,sizeof(frequency));

    startSensorCapture(activeChannel);

    return 0;
}

int startSensorCapture(int Sensor)
{

    MeasureInit(ptrTimer3Ref,TimerChannel);

    // select input
    moiSetChannel(Sensor);

    return Sensor;
}

void printMoisture() {
	I( "Measured channel 1: %d Hz",
        (int)frequency[MOISTURE_MEASURE_CHANNEL0_ACTIVE]);
    I( "Measured channel 2: %d Hz",
        (int)frequency[MOISTURE_MEASURE_CHANNEL1_ACTIVE]);
    I( "Measured channel 3: %d Hz",
        (int)frequency[MOISTURE_MEASURE_CHANNEL2_ACTIVE]);
    I( "Measured channel 4: %d Hz",
        (int)frequency[MOISTURE_MEASURE_CHANNEL3_ACTIVE]);
    I( "Measured channel 5: %d Hz",
        (int)frequency[MOISTURE_MEASURE_CHANNEL4_ACTIVE]);
}

int MeasureInit(TIM_HandleTypeDef * ptrTimerRef,uint32_t channel) {
    int retval = -1;

    D("start measure");

    /* TIM enable counter */
    if(ptrTimerRef) {
        if (HAL_OK != (retval = HAL_TIM_IC_Start_IT(ptrTimerRef, channel))) {
            //printf("FAILED: timer start, erro: %d",retval);
            E( "failed to start timer!");
        }
    }
    return retval;
}

void moiEnableSensor() {
    //osDelay(20);
    HAL_GPIO_WritePin(GPIOB,MOISTURE_SENS_PIN_ENABLE,GPIO_PIN_SET);
}

void moiDisableSensor() {
    HAL_GPIO_WritePin(GPIOB,MOISTURE_SENS_PIN_ENABLE,GPIO_PIN_RESET);
}

void moiSetChannel(int channel)
{
    volatile int16_t tSensorSelect = 0;

    HAL_TIM_IC_Stop_IT(ptrTimer3Ref,TIM_CHANNEL_2);
    moiDisableSensor();
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
    moiEnableSensor();
    HAL_TIM_IC_Start_IT(ptrTimer3Ref,TIM_CHANNEL_2);
}

/*
void moiBrokerMessage(char *buf, uint8_t length)
{
    char send[SPI_XFER_SIZE];
    switch(buf[0]) {
    case BRK_MSG_SPI_ID_SENS_VALUE:
        send[1] = (uint8_t)getSensorFrequency(buf[1])/(uint8_t)1000;
        send[0] = buf[1];
        spiSend(BRK_MSG_SPI_ID_SENS_VALUE_RSP,send);
        break;
    case BRK_MSG_SPI_ID_SENS_VALUE_RSP:
        break;
    default:
        break;
    }
}*/


void MoistureTask() {
    if (stateRegister == MOISTURE_MEASURE_STATE_ACTIVE) {
        frequency[activeChannel] = getFrequencyOfChannel();
        activeChannel++;
        if (activeChannel > MOISTURE_MEASURE_CHANNEL_MAX)
            activeChannel = MOISTURE_MEASURE_CHANNEL0_ACTIVE;
        moiSetChannel(activeChannel);
    } else {
        E( "MOI inactive state");
    }
}

void procSensor(void const * argument) {
	PROCRUNNING;
	osEvent event;
	do {
		event = osMessageGet(sensorCtrlQueue,500);
		switch(event.status) {
		case osEventMessage: {
			D("message: 0x%02x",event.value.p);
			SpiBuffer spi;
			memset(&spi,0,sizeof(spi));
			stSensorCmd *cmd = event.value.p;
			cmd->value = getSensorFrequency(cmd->sensor);
			spi.d[1] = cmd->value;
			spi.d[0] = BRK_MSG_SPI_ID_SENS_VALUE_RSP;
			SpiSend(&cmd);
			printMoisture();
			break;
		}
		case osEventTimeout: {
			MoistureTask();
			break;
		}
		default:
			MoistureTask();
			break;

		}
	}while(1);
}

/**
  * @}
  */


