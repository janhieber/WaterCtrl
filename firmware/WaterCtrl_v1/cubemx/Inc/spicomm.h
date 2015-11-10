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
#include <string.h>

/*! size of the SPI send queue */
#define SPI_SENDQUEUE_SIZE (SPI_XFER_SIZE * 16)
/*! size of the SPI receive queue */
#define SPI_RECVQUEUE_SIZE (SPI_XFER_SIZE * 16)

/** @brief Init SPI queues
 */
void spiQueueInit();

/** @brief adds data to SPI send queue
 *  @param id message ID as byte as specified in firmware/SPECS.md
 *  @param msg message to send as 0 terminated string. Becareful
 *      to avoid 0 when sending data other than ASCII
 */
void spiSend(uint8_t id, char *msg);

void spiDebug(UART_HandleTypeDef *huart);

#endif /* SPICOMM_H_ */

/**
  * @}
  */
/**
  * @}
  */
