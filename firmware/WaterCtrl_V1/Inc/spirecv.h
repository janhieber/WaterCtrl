/* ---------------------------------------------------------------------------
** This software is in the public domain, furnished "as is", without technical
** support, and with no warranty, express or implied, as to its usefulness for
** any purpose.
**
** spirecv.h
** Functions for SPI receiver.
** - init/handle interrupts/DMA
** - act on received messages
** - send queued messages
**
** Author: Jan Hieber <mail@janhieber.net>
** -------------------------------------------------------------------------*/

#ifndef SPIRECV_H_
#define SPIRECV_H_

#include <stdint.h>


#define SPI_RECSIZE 128
uint8_t spi_recvbuf[SPI_RECSIZE];


#endif /* SPIRECV_H_ */
