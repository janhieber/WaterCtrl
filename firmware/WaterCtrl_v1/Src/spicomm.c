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

#include <log.h>
#include <spicomm.h>
#include <mxconstants.h>
#include <stuff.h>

extern SPI_HandleTypeDef hspi1;

// debug
extern UART_HandleTypeDef huart1;

/*! these are pointers for sending and receiving, pointed
 *  to the queue. Should always increment by SPI_XFER_SIZE
 *  to not shoot over the end!
 *  current start end end are defined in index vars! */
static volatile uint8_t *spiSendBuf;
static volatile uint8_t *spiRecvBuf;

/*! these are circular buffers for sending end receiving
 *  messages end with a newline ('\n') char */
static volatile uint8_t spiSendQueue[SPI_SENDQUEUE_SIZE];
static volatile uint8_t spiRecvQueue[SPI_RECVQUEUE_SIZE][SPI_XFER_SIZE + 1];

/*! Index of current queue start and end.
 *  Need to be multiples of SPI_XFER_SIZE */
static volatile uint32_t spiSendQueueBegin;
static volatile uint32_t spiSendQueueEnd;
static volatile uint32_t spiRecvQueueIdx;

static bool spiReady = false;

void spiQueueInit() {
	// init send queue
	for (uint32_t i = 0; i < SPI_SENDQUEUE_SIZE; i++)
		spiSendQueue[i] = 0x31;
	// init recv queue
	for (uint32_t i = 0; i < SPI_RECVQUEUE_SIZE; i++)
		for (uint32_t j = 0; j < SPI_XFER_SIZE; j++)
			spiRecvQueue[i][j] = 0x0;
	// reset begin / end marks
	spiSendQueueBegin = 0;
	spiSendQueueEnd = 0;
	spiRecvQueueIdx = 0;
	// init send/recv pointers
	spiSendBuf = spiSendQueue;
	spiRecvBuf = (uint8_t *)&spiRecvQueue[0][1];
	// set ready flag
	spiReady = true;

	// start transfer system
	//HAL_SPI_Receive_IT(&hspi1,(uint8_t *)spiRecvBuf, SPI_XFER_SIZE);
	HAL_SPI_TransmitReceive_IT(&hspi1,(uint8_t *)spiSendBuf,(uint8_t *)spiRecvBuf, SPI_XFER_SIZE);
}

void spiDebug(UART_HandleTypeDef *huart) {
	// HAL_UART_Transmit(huart, spiSendQueue, SPI_SENDQUEUE_SIZE, 100);
	// HAL_UART_Transmit(huart, "\r\n", 2, 100);
}

void spiSend(uint8_t id, char *msg) {
	if (unlikely(!spiReady))
		return;

	// We save this var because ISR can change it while function runs.
	// We only need a constant value for this function
	uint32_t _spiSendQueueBegin = spiSendQueueBegin;

	// msglen is string len + id
	uint8_t msglen = strlen(msg);

	// datalen is msglen + id rounded to SPI_XFER_SIZE
	uint8_t datalen = msglen + 1;
	if (datalen % SPI_XFER_SIZE)
		datalen = SPI_XFER_SIZE * (datalen / SPI_XFER_SIZE + 1);

	if (spiSendQueueEnd >= _spiSendQueueBegin) {
		if (spiSendQueueEnd + datalen <= SPI_SENDQUEUE_SIZE) {
			// if fits in buffer
			spiSendQueue[spiSendQueueEnd] = id;
			strncpy((char *restrict) & (spiSendQueue[spiSendQueueEnd + 1]), msg,
					msglen);
			// set new end
			spiSendQueueEnd += datalen;
			if (spiSendQueueEnd >= SPI_SENDQUEUE_SIZE)
				spiSendQueueEnd = 0;
		} else {
			// if it does not fit in buffer to end
			uint8_t remaining =
					datalen - (SPI_SENDQUEUE_SIZE - spiSendQueueEnd);
			if (remaining <= _spiSendQueueBegin) {
				// copy id
				spiSendQueue[spiSendQueueEnd] = id;
				// copy data
				strncpy((char *restrict) & (spiSendQueue[spiSendQueueEnd + 1]),
						msg, SPI_SENDQUEUE_SIZE - (spiSendQueueEnd + 1));
				strncpy((char *restrict)spiSendQueue,
						&(msg[SPI_SENDQUEUE_SIZE - (spiSendQueueEnd + 1)]),
						msglen - (SPI_SENDQUEUE_SIZE - (spiSendQueueEnd + 1)));
				// set new end
				spiSendQueueEnd = remaining;
			} else {
				LogUart(LogError, "SPI send buf full! 1");
			}
		}
	} else {
		// if it fits before begin
		if (datalen <= (_spiSendQueueBegin - spiSendQueueEnd)) {
			// if fits in buffer
			spiSendQueue[spiSendQueueEnd] = id;
			strncpy((char *restrict) & (spiSendQueue[spiSendQueueEnd + 1]), msg,
					msglen);
			// set new end
			spiSendQueueEnd += datalen;
		} else
			LogUart(LogError, "SPI send buf full! 4");
	}
	if(HAL_SPI_GetState(&hspi1) == HAL_SPI_STATE_BUSY_TX){
		Log(LogDebug,"SPI: queued");
	} else {
		//HAL_SPI_Transmit_IT(&hspi1,(uint8_t *)spiSendBuf, SPI_XFER_SIZE);
		HAL_SPI_TransmitReceive_IT(&hspi1,(uint8_t *)spiSendBuf,(uint8_t *)spiRecvBuf, SPI_XFER_SIZE);
		Log(LogDebug,"SPI: SendNew Message : %d",hspi1.TxXferCount);
	}
}

bool spiReceive(char *buf) {
	// check if we have new data
	volatile uint32_t i = 0;
	while (i < SPI_RECVQUEUE_SIZE) {
		if (spiRecvQueue[i][0] == 0xaa)
			break;
		i++;
	}

	// if we found something, process it
	if (spiRecvQueue[i][0] == 0xaa) {
		strncpy((char *restrict)buf, (char *)&spiRecvQueue[i][1],
				SPI_XFER_SIZE);
		// clear flag, buffer is free
		spiRecvQueue[i][0] = 0x00;
		return true;
	}
	return false;
}

void HAL_SPI_ErrorCallback(SPI_HandleTypeDef *hspi) {
	LogUart(LogError, "SPI ErrorCode: 0x%02x", (unsigned int)hspi->ErrorCode);
	switch(hspi->ErrorCode) {
	case HAL_SPI_ERROR_OVR:
		// setup next XFER
		spiQueueInit();
		HAL_SPI_TransmitReceive_IT(&hspi1, (uint8_t *)spiSendBuf,
				(uint8_t *)spiRecvBuf, SPI_XFER_SIZE);
		Log(LogInfo,"SPI: Restart tranfer");

		break;
	default:
		LogUart(LogInfo,"SPI: error ignored");
		break;
	}
}

/** @brief SPI receive and transmit complete callback
 */
void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi) {
	if (unlikely(!spiReady))
		return;

	Log(LogInfo,"HAL_SPI_TxRxCpltCallback");

	// if we received nothing, let the recv buffer as is
	if (spiRecvQueue[spiRecvQueueIdx][1] != 0) {
		// mark current buffer as ready to process
		spiRecvQueue[spiRecvQueueIdx][0] = 0xaa;
		// next index
		if (spiRecvQueueIdx + 1 > SPI_RECVQUEUE_SIZE)
			spiRecvQueueIdx = 0;
		else
			spiRecvQueueIdx++;
		// check if next segment is empty to receive data
		if (spiRecvQueue[spiRecvQueueIdx][0] != 0 ) {
			// if there is data, receive buf is too small
			Log(LogError, "SPI-TxRx: receive buf too small!");
		}
		spiRecvBuf = (uint8_t *)&spiRecvQueue[spiRecvQueueIdx][1];
	}
	//    // delete last sent bytes
	//    for (uint32_t i = spiSendQueueBegin;
	//         i < (spiSendQueueBegin + SPI_XFER_SIZE); i++)
	//        spiSendQueue[i] = 0;

	// set new send buffer
	if (spiSendQueueBegin < spiSendQueueEnd) {
		if ((spiSendQueueBegin + SPI_XFER_SIZE) <= spiSendQueueEnd) {
			// if end not reached
			spiSendQueueBegin += SPI_XFER_SIZE;
			spiSendBuf = &(spiSendQueue[spiSendQueueBegin]);
			//HAL_SPI_Transmit_IT(&hspi1,(uint8_t *)spiSendBuf, SPI_XFER_SIZE);
			//HAL_SPI_TransmitReceive_IT(&hspi1,(uint8_t *)spiSendBuf, (uint8_t *)spiRecvBuf, SPI_XFER_SIZE);
			Log(LogDebug,"SPI-TxRx: start begin %d , end %d",spiSendQueueBegin,spiSendQueueEnd);
		} else {
			Log(LogDebug,"SPI-TxRx: init start begin %d , end %d",spiSendQueueBegin,spiSendQueueEnd);
		}
		HAL_SPI_TransmitReceive_IT(&hspi1,(uint8_t *)spiSendBuf, (uint8_t *)spiRecvBuf, SPI_XFER_SIZE);
	} else {
		if (spiSendQueueBegin != spiSendQueueEnd) {
			if ((spiSendQueueBegin + SPI_XFER_SIZE) < SPI_SENDQUEUE_SIZE) {
				// if not overlapping
				spiSendQueueBegin += SPI_XFER_SIZE;
				spiSendBuf = &(spiSendQueue[spiSendQueueBegin]);
				//HAL_SPI_Transmit_IT(&hspi1,(uint8_t *)spiSendBuf, SPI_XFER_SIZE);
				//HAL_SPI_TransmitReceive_IT(&hspi1,(uint8_t *)spiSendBuf, (uint8_t *)spiRecvBuf, SPI_XFER_SIZE);
				Log(LogDebug,"SPI-TxRx: continues begin %d , end %d",spiSendQueueBegin,spiSendQueueEnd);

			} else {
				// if overlapping
				spiSendQueueBegin = 0;
				spiSendBuf = &(spiSendQueue[spiSendQueueBegin]);
				HAL_SPI_TransmitReceive_IT(&hspi1,(uint8_t *)spiSendBuf, (uint8_t *)spiRecvBuf, SPI_XFER_SIZE);
				Log(LogDebug,"SPI-TxRx: overlapped begin %d , end %d",spiSendQueueBegin,spiSendQueueEnd);
			}
		} else {
			// if begin = end
			spiSendBuf = &(spiSendQueue[spiSendQueueBegin]);
			Log(LogDebug,"SPI-TxRx: send done");
			// Wait for next message
			//HAL_SPI_Receive_IT(&hspi1,(uint8_t *)spiRecvBuf, SPI_XFER_SIZE);
		}
	}
//	HAL_SPI_TransmitReceive_IT(&hspi1, (uint8_t *)spiSendBuf,
//			(uint8_t *)spiRecvBuf, SPI_XFER_SIZE);
	Log(LogInfo,"HAL_SPI_TxRxCpltCallback end");
}

/** @brief SPI receive and transmit complete callback
 */
void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef *hspi) {
	if (unlikely(!spiReady))
		return;
	Log(LogDebug,"Receive callback");
	// mark current buffer as ready to process
	spiRecvQueue[spiRecvQueueIdx][0] = 0xaa;
	// next index
	if (spiRecvQueueIdx + 1 > SPI_RECVQUEUE_SIZE)
		spiRecvQueueIdx = 0;
	else
		spiRecvQueueIdx++;
	// check if next segment is empty to receive data
	if (spiRecvQueue[spiRecvQueueIdx][0] != 0 ) {
		// if there is data, receive buf is too small
		Log(LogError, "SPI receive buf too small!");
	}
	spiRecvBuf = (uint8_t *)&spiRecvQueue[spiRecvQueueIdx][1];
	//HAL_SPI_Receive_IT(&hspi1,(uint8_t *)spiRecvBuf, SPI_XFER_SIZE);
	HAL_SPI_TransmitReceive_IT(&hspi1, (uint8_t *)spiSendBuf,
			(uint8_t *)spiRecvBuf, SPI_XFER_SIZE);
	Log(LogDebug,"Receive callback end");
}

/** @brief SPI receive and transmit complete callback
 */
void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi) {
	if (unlikely(!spiReady))
		return;
	Log(LogDebug,"Transmit callback");

	// wait for next message
	// HAL_SPI_Receive_IT(&hspi1,(uint8_t *)spiRecvBuf, SPI_XFER_SIZE);

	// set new send buffer
	if (spiSendQueueBegin < spiSendQueueEnd) {
		if ((spiSendQueueBegin + SPI_XFER_SIZE) <= spiSendQueueEnd) {
			// if end not reached
			spiSendQueueBegin += SPI_XFER_SIZE;
			spiSendBuf = &(spiSendQueue[spiSendQueueBegin]);
			//HAL_SPI_Transmit_ITReceive(&hspi1,(uint8_t *)spiSendBuf, SPI_XFER_SIZE);
			//HAL_SPI_TransmitReceive_IT(&hspi1,(uint8_t *)spiSendBuf, (uint8_t *)spiRecvBuf, SPI_XFER_SIZE);
			Log(LogDebug,"SPI: start begin %d , end %d",spiSendQueueBegin,spiSendQueueEnd);
		}
	} else {
		if (spiSendQueueBegin != spiSendQueueEnd) {
			if ((spiSendQueueBegin + SPI_XFER_SIZE) < SPI_SENDQUEUE_SIZE) {
				// if not overlapping
				spiSendQueueBegin += SPI_XFER_SIZE;
				spiSendBuf = &(spiSendQueue[spiSendQueueBegin]);
				//HAL_SPI_Transmit_IT(&hspi1,(uint8_t *)spiSendBuf, SPI_XFER_SIZE);
				//HAL_SPI_TransmitReceive_IT(&hspi1,(uint8_t *)spiSendBuf, (uint8_t *)spiRecvBuf, SPI_XFER_SIZE);
				Log(LogDebug,"SPI: continues begin %d , end %d",spiSendQueueBegin,spiSendQueueEnd);

			} else {
				// if overlapping
				spiSendQueueBegin = 0;
				spiSendBuf = &(spiSendQueue[spiSendQueueBegin]);
				HAL_SPI_TransmitReceive_IT(&hspi1,(uint8_t *)spiSendBuf, (uint8_t *)spiRecvBuf, SPI_XFER_SIZE);
				Log(LogDebug,"SPI: overlapped begin %d , end %d",spiSendQueueBegin,spiSendQueueEnd);
			}
		} else {
			// if begin = end
			spiSendBuf = &(spiSendQueue[spiSendQueueBegin]);
			Log(LogDebug,"SPI: send done");
			// Wait for next message
			//HAL_SPI_Receive_IT(&hspi1,(uint8_t *)spiRecvBuf, SPI_XFER_SIZE);

		}
	}

	HAL_SPI_TransmitReceive_IT(&hspi1,(uint8_t *)spiSendBuf, (uint8_t *)spiRecvBuf, SPI_XFER_SIZE);
	Log(LogDebug,"Transmit callback end");
}
/**
 * @}
 */
/**
 * @}
 */
