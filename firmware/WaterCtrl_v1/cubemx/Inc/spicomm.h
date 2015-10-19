/** @file spicomm.h
 *  @brief Functions for SPI receiver.
 *      - init/handle interrupts/DMA
 *      - act on received messages
 *      - send queued messages
 *
 *  @author Jan Hieber <mail@janhieber.net>
 */

#ifndef SPICOMM_H_
#define SPICOMM_H_

#include <stdint.h>
#include <string.h>

/*! size of the SPI send queue */
#define SPI_SENDQUEUE_SIZE (SPI_XFER_SIZE*128)
/*! size of the SPI receive queue */
#define SPI_RECVQUEUE_SIZE (SPI_XFER_SIZE*128)



void spiQueueInit();


#endif /* SPICOMM_H_ */
