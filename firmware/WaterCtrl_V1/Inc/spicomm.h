/* ---------------------------------------------------------------------------
** This software is in the public domain, furnished "as is", without technical
** support, and with no warranty, express or implied, as to its usefulness for
** any purpose.
**
** spicomm.h
** Functions for SPI receiver.
** - init/handle interrupts/DMA
** - act on received messages
** - send queued messages
**
** Author: Jan Hieber <mail@janhieber.net>
** -------------------------------------------------------------------------*/

#ifndef SPICOMM_H_
#define SPICOMM_H_

#include <stdint.h>


#define SPI_RECSIZE 128
uint8_t spi_recvbuf[SPI_RECSIZE];

typedef enum {
    SpiQueueSend = 0x1,
    SpiQueueRecv = 0x1
} SpiQueue;


void spiQueueInit(void);
void spiQueuePush(SpiQueue spiqueue, char *msg);
char* spiQueuePop(SpiQueue spiqueue);




#endif /* SPICOMM_H_ */
