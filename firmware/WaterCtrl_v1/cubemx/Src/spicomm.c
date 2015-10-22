/** @file spicomm.c
 *
 *  @author Jan Hieber <mail@janhieber.net>
 */

#include "stm32f1xx_hal.h"

#include <stdlib.h>
#include <stdbool.h>

#include <log.h>
#include <spicomm.h>
#include <mxconstants.h>

extern SPI_HandleTypeDef hspi1;

/*! these are pointers for sending and receiving, pointed
 *  to the queue. Should always increment by SPI_XFER_SIZE
 *  to not shoot over the end!
 *  current start end end are defined in index vars! */
static volatile uint8_t *spiSendBuf;
static volatile uint8_t *spiRecvBuf;

/*! these are circular buffers for sending end receiving
 *  messages end with a newline ('\n') char */
static volatile uint8_t spiSendQueue[SPI_SENDQUEUE_SIZE];
static volatile uint8_t spiRecvQueue[SPI_RECVQUEUE_SIZE];

/*! Index of current queue start and end.
 *  Need to be multiples of SPI_XFER_SIZE */
static volatile uint32_t spiSendQueueBegin;
static volatile uint32_t spiSendQueueEnd;
static volatile uint32_t spiRecvQueueBegin;
static volatile uint32_t spiRecvQueueEnd;

/** @brief Init SPI queues
 */
void spiQueueInit() {
    // init send queue
    for (uint32_t i = 0; i < SPI_SENDQUEUE_SIZE; i++)
        spiSendQueue[i] = 0;
    // init recv queue
    for (uint32_t i = 0; i < SPI_RECVQUEUE_SIZE; i++)
        spiRecvQueue[i] = 0;
    // reset begin / end marks
    spiSendQueueBegin = 0;
    spiSendQueueEnd = 0;
    spiRecvQueueBegin = 0;
    spiRecvQueueEnd = 0;
    // init send/recv pointers
    spiSendBuf = spiSendQueue;
    spiRecvBuf = spiRecvQueue;
}

void spiSend(char *msg){
    uint8_t msglen = strlen(msg);
    if (spiSendQueueEnd >= spiSendQueueBegin){
        if(spiSendQueueEnd + msglen <= SPI_SENDQUEUE_SIZE){
            // if fits in buffer
            strcpy((char * restrict)&(spiSendQueue[spiSendQueueEnd]), msg);
            // set new end
            if (msglen % SPI_XFER_SIZE)
                spiSendQueueEnd += SPI_XFER_SIZE * ((msglen % SPI_XFER_SIZE) +1);
            else
                spiSendQueueEnd += msglen;
        }else{
            // if it does not fit in buffer to end
            uint8_t remaining = msglen - (SPI_SENDQUEUE_SIZE - spiSendQueueEnd);
            if (remaining < spiSendQueueBegin) {
                strncpy((char * restrict)&(spiSendQueue[spiSendQueueEnd]), msg, SPI_SENDQUEUE_SIZE - spiSendQueueEnd);
                strncpy((char * restrict)spiSendQueue, &(msg[SPI_SENDQUEUE_SIZE - spiSendQueueEnd +1]), remaining);
                if (remaining % SPI_XFER_SIZE)
                    spiSendQueueEnd += SPI_XFER_SIZE * ((remaining % SPI_XFER_SIZE) +1);
                else
                    spiSendQueueEnd += remaining;
            }else {
                Log(LogError, "SPI send buf full!");
            }
        }
    }
}

void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi) {
    // set new recv buffer
    if (spiRecvQueueEnd < spiRecvQueueBegin) {
        if ((spiRecvQueueEnd + SPI_XFER_SIZE) < spiRecvQueueBegin) {
            // if end not reached
            spiRecvQueueEnd += SPI_XFER_SIZE;
            spiRecvBuf = &(spiRecvQueue[spiRecvQueueEnd]);
        } else
            Log(LogError, "Spi Recv buffer full!");
    } else {
        if ((spiRecvQueueEnd + SPI_XFER_SIZE) < SPI_RECVQUEUE_SIZE) {
            // if not overlapping
            spiRecvQueueEnd += SPI_XFER_SIZE;
            spiRecvBuf = &(spiRecvQueue[spiRecvQueueEnd]);
        } else {
            // if overlapping
            if (spiRecvQueueBegin != 0) {
                spiRecvQueueEnd = 0;
                spiRecvBuf = &(spiRecvQueue[spiRecvQueueEnd]);
            } else
                Log(LogError, "Spi Recv buffer full!");
        }

    }

    // delete last sent bytes
    for (uint32_t i = spiSendQueueBegin; i < (spiSendQueueBegin + SPI_XFER_SIZE); i++)
        spiSendQueue[i] = 0;
    // set new send pointer
    if (spiSendQueueBegin < spiSendQueueEnd) {
        if ((spiSendQueueBegin + SPI_XFER_SIZE) < spiSendQueueEnd) {
            // if end not reached
            spiSendQueueBegin += SPI_XFER_SIZE;
            spiSendBuf = &(spiSendQueue[spiSendQueueBegin]);
        }
    } else {
        if (spiSendQueueBegin != spiSendQueueEnd) {
            if ((spiSendQueueBegin + SPI_XFER_SIZE) < SPI_SENDQUEUE_SIZE) {
                // if not overlapping
                spiSendQueueBegin += SPI_XFER_SIZE;
                spiSendBuf = &(spiSendQueue[spiSendQueueBegin]);
            } else {
                // if overlapping
                spiSendQueueBegin = 0;
                spiSendBuf = &(spiSendQueue[spiSendQueueBegin]);
            }
        }
    }

    // setup next XFER
    HAL_SPI_TransmitReceive_IT(&hspi1, (uint8_t *)spiSendBuf, (uint8_t *)spiRecvBuf, SPI_XFER_SIZE);
}
