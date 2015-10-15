/* ---------------------------------------------------------------------------
 ** This software is in the public domain, furnished "as is", without technical
 ** support, and with no warranty, express or implied, as to its usefulness for
 ** any purpose.
 **
 ** spicomm.c
 ** *see header file*
 **
 ** Author: Jan Hieber <mail@janhieber.net>
 ** -------------------------------------------------------------------------*/

#include <stdlib.h>
#include <stdbool.h>

#include <spicomm.h>
#include "stm32f1xx_hal.h"

extern SPI_HandleTypeDef hspi1;
uint8_t spisend[16];
uint8_t spirecv[16];
bool uartsend_new;

char spiSendBuf[SPI_SENDBUF_SIZE][STR_MAX_SIZE];
char spiRecvBuf[SPI_RECVBUF_SIZE][STR_MAX_SIZE];
uint8_t spiRecvIdx;

void spiQueueInit() {
    int i, j;
    for (i = 0; i < SPI_SENDBUF_SIZE; i++)
        for (j = 0; j < STR_MAX_SIZE; j++)
            spiSendBuf[i][j] = 0;
    for (i = 0; i < SPI_RECVBUF_SIZE; i++)
        for (j = 0; j < STR_MAX_SIZE; j++)
            spiRecvBuf[i][j] = 0;
    spiRecvIdx = 0;
}

void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef *hspi) {
    HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_9);
    if (hspi->Instance == hspi1.Instance) {

        //HAL_SPI_Receive_IT(&hspi1, spi_recvbuf, SPI_RECSIZE);

    }
}

void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi)
{
    strcpy((char *)spisend, "Hello IT\r\n");
    HAL_SPI_TransmitReceive_IT(&hspi1, spisend, spirecv, sizeof(spisend));
    uartsend_new = true;
}
