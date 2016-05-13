/** @file spicomm.h
 *  @brief Functions for SPI.
 *      - init/handle interrupts
 *      - act on received messages
 *      - send queued messages
 *
 *  @author Jan Hieber <mail@janhieber.net>
 */

/** @addtogroup Communication
  * @{
  */

/** @addtogroup SPI
  * @{
  */

#ifndef SPICOMM_H_
#define SPICOMM_H_

#include "stm32f1xx_hal.h"

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#define SPI_XFER_SIZE 6 // in bytes
#define SPI_SENDQUEUE_SIZE 20 // in buffer elements
#define SPI_RECVQUEUE_SIZE 20 // in buffer elements

typedef struct SpiBuffer {
	uint8_t d[SPI_XFER_SIZE];
} SpiBuffer;



bool SpiSend(SpiBuffer* data);


#endif /* SPICOMM_H_ */

/**
  * @}
  */
/**
  * @}
  */
