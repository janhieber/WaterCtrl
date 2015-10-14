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


#define SPI_SENDBUF_SIZE 16
#define SPI_RECVBUF_SIZE 16

void spiQueueInit();


#endif /* SPICOMM_H_ */
