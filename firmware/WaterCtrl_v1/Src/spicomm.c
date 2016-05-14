/** @file spicomm.c
 *
 *  @author Jan Hieber <mail@janhieber.net>
 */

/** @addtogroup Communication
  * @{
  */

/** @addtogroup SPI
  * @{
  */

#include "stm32f1xx_hal.h"

#include <stdlib.h>
#include <stdbool.h>

#include <spicomm.h>
#include <mxconstants.h>
#include "usart.h"
#include "spi.h"
#include "cmsis_os.h"
#include "motors.h"
#include "moistureMeasure.h"


#define SYSNAME "SpiBroker"


static const SpiBuffer sendDummy = {0, };

// handles for queues
static osMessageQId spiSendQueue;
static osMessageQId spiRecvQueue;
// definitions of queues
osMessageQDef(_spiSendQueue, SPI_SENDQUEUE_SIZE, SpiBuffer*);
osMessageQDef(_spiRecvQueue, SPI_RECVQUEUE_SIZE, SpiBuffer*);

// pools for data
static osPoolId  spiSendPool;
static osPoolId  spiRecvPool;
osPoolDef(_spiSendPool, SPI_SENDQUEUE_SIZE, SpiBuffer);
osPoolDef(_spiRecvPool, SPI_RECVQUEUE_SIZE, SpiBuffer);


// here we store the buffers for the current transfer
volatile SpiBuffer* send = NULL;
volatile SpiBuffer* recv = NULL;

void initSpi(void) {
	INITBEGIN;

	// init queues
	spiSendQueue = osMessageCreate(osMessageQ(_spiSendQueue), NULL);
	spiRecvQueue = osMessageCreate(osMessageQ(_spiRecvQueue), NULL);

	// init data pools
	spiSendPool = osPoolCreate(osPool(_spiSendPool));
	spiRecvPool = osPoolCreate(osPool(_spiRecvPool));



	INITEND;
}

void procSpiBroker(void const * argument){
	PROCRUNNING;

    // start transfer system
	// get empty receive buffer
	recv = (SpiBuffer*)osPoolAlloc(spiRecvPool);
	// send dummy in first message
	send = (SpiBuffer*)&sendDummy;
	HAL_SPI_TransmitReceive_IT(&hspi1, (uint8_t*)send->d, (uint8_t*)recv->d, SPI_XFER_SIZE);


    osEvent evt;

	while(true) {
		// here we check for incoming messages from SPI
		evt = osMessageGet(spiRecvQueue, osWaitForever);

		// we got one
		if (evt.status == osEventMessage) {
			// here we decide what to do
			SpiBuffer* recvMsg = (SpiBuffer*)evt.value.p;

			switch(recvMsg->d[0]){
			case SPI_ID_MOTOR_CTRL:
				D("motor command received");
				// pack the message in an MotorCmd and send it to Motor system
				MotorCmd* cmd = (MotorCmd*)osPoolAlloc(motorCtrlPool);
				cmd->motor = recvMsg->d[1];
				cmd->time = recvMsg->d[2];
				cmd->speed = recvMsg->d[3];
				osMessagePut(motorCtrlQueue, (uint32_t)cmd, 0);
				break;

			case SPI_ID_MOIST_REQ:
				D("moisture request received");

				break;

			case SPI_ID_NOP:
				break;
			default:
				E("unknown data received %s",recvMsg->d);
				recvMsg->d[1] = 0x31;
				SpiSend(recvMsg);
				break;
			}

			// at least we free this pool element
			osPoolFree(spiRecvPool, recvMsg);
		}
	}
}

/**
 * Send data via SPI to RPI.
 * This is for external use by other functions.
 */
bool SpiSend(SpiBuffer* data){
	// allocate space
	SpiBuffer* buf = (SpiBuffer*)osPoolAlloc(spiSendPool);
	// copy data
	memcpy(buf, data, sizeof(SpiBuffer));
	// send to queue
	osMessagePut(spiSendQueue, (uint32_t)buf, 250);

	return true;
}


static bool bufferEmpty(SpiBuffer* buf) {
	for(uint8_t i=0; i<SPI_XFER_SIZE; i++){
		if(buf->d[i] == 0)
			return true;
	}
	return false;
}


void HAL_SPI_ErrorCallback(SPI_HandleTypeDef *hspi) {
    E("ErrorCode: 0x%08x", (unsigned int)hspi->ErrorCode);
}

/** @brief SPI receive complete callback
 */
void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi) {

	// if this was an other SPI, we quit
	if(hspi != &hspi1)
		return;

	D("Transmit receive callback");

	// receive stuff
	{
		// check if we received data on last transfer
		if(!bufferEmpty((SpiBuffer*)recv)){
			// not empty, so post the pointer to the buffer in the queue
			// for further processing
			if(osMessagePut(spiRecvQueue, (uint32_t)recv, 0) != osOK)
				E("SPI recv queue is full!");

			// get new receive buffer for next message
			recv = (SpiBuffer*)osPoolAlloc(spiRecvPool);
		}

		// if message was empty, we simply reuse the buffer
	}

	// send stuff
	{
		// free the last send buffer
		osPoolFree(spiSendPool, (SpiBuffer*)send);

		// check if there is new data to send
		send = (SpiBuffer*)&sendDummy;

		osEvent evt = osMessageGet(spiSendQueue, 0);
		if (evt.status == osEventMessage) {
			send = (SpiBuffer*)evt.value.p;
		}
	}

    // start next transfer
    HAL_SPI_TransmitReceive_IT(&hspi1, (uint8_t*)send->d, (uint8_t*)recv->d, SPI_XFER_SIZE);
}

/**
  * @}
  */
/**
  * @}
  */
