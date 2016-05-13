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


#define SYSNAME "SpiBroker"


static const SpiBuffer sendDummy = {0, };

// handles for queues
static osMessageQId spiSendQueue;
static osMessageQId spiRecvQueue;
// definitions of queues
osMessageQDef(_spiSendQueue, SPI_SENDQUEUE_SIZE, SpiBuffer);
osMessageQDef(_spiRecvQueue, SPI_RECVQUEUE_SIZE, SpiBuffer);

// pools for data
static osPoolId  spiSendPool;
static osPoolId  spiRecvPool;
osPoolDef(_spiSendPool, SPI_SENDQUEUE_SIZE, SpiBuffer);
osPoolDef(_spiRecvPool, SPI_RECVQUEUE_SIZE, SpiBuffer);



void initSpi() {
	INITBEGIN;

	// init queues
	spiSendQueue = osMessageCreate(osMessageQ(_spiSendQueue), NULL);
	spiRecvQueue = osMessageCreate(osMessageQ(_spiRecvQueue), NULL);

	// init data pools
	spiSendPool = osPoolCreate(osPool(_spiSendPool));
	spiRecvPool = osPoolCreate(osPool(_spiRecvPool));

	// init dummy, we send it when there is nothing to send
	for(uint8_t i=0; i<SPI_XFER_SIZE; i++)
		sendDummy[i] = 0;


	INITEND;
}

void procSpiBroker(void const * argument){

	PROCRUNNING;


    // start transfer system
	{
		SpiBuffer* recvBuf = (SpiBuffer*)osPoolAlloc(spiRecvPool);
		HAL_SPI_TransmitReceive_IT(&hspi1, &sendDummy, recvBuf, SPI_XFER_SIZE);
	}


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
				// pack the message in an MotorCmd and send it to Motor system
				MotorCmd* cmd = (MotorCmd*)osPoolAlloc(motorCtrlPool);
				cmd->motor = recvMsg->d[1];
				cmd->time = recvMsg->d[2];
				cmd->speed = recvMsg->d[3];
				osMessagePut(motorCtrlQueue, (uint32_t)cmd, 0);
				break;

			case SPI_ID_MOIST_REQ:


				break;

			case SPI_ID_NOP:
				break;
			default:
				EE("unknown data received");
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
    EE("ErrorCode: 0x%08x", (unsigned int)hspi->ErrorCode);
}

/** @brief SPI receive complete callback
 */
void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi) {

	// if this was an other SPI, we quit
	if(hspi != &hspi1)
		return;

	// here we store the buffers for new transfer
	SpiBuffer* send = NULL;
	SpiBuffer* recv = NULL;

	// receive stuff
	{
		// get the last used recvbuffer
		SpiBuffer* lastRecvBuf = hspi->pRxBuffPtr;

		// check if we received data on last transfer
		if(!bufferEmpty(lastRecvBuf)){
			// not empty, so put the pointer to the buffer in the queue
			// for further processing
			osEvent evt;
			evt.value.p = lastRecvBuf;
			osMessagePut(spiRecvQueue);

			// get new receive buffer for next message
			recv = (SpiBuffer*)osPoolAlloc(spiRecvPool);
		}else{
			// so the message was empty, in this case we simply reuse it
			recv = lastRecvBuf;
		}
	}

	// send stuff
	{
		// check which buffer was send the last time and free it
		SpiBuffer* lastSendBuf = hspi->pTxBuffPtr;
		osPoolFree(spiSendPool, lastSendBuf);

		// check if there is new data to send
		send = &sendDummy;
		osEvent evt = osMessageGet(spiSendQueue, 0);
		if (evt.status == osEventMessage) {
			send = (SpiBuffer*)evt.value.p;
		}
	}

    // start next transfer
    HAL_SPI_TransmitReceive_IT(&hspi1, send, recv, SPI_XFER_SIZE);
}

/**
  * @}
  */
/**
  * @}
  */
