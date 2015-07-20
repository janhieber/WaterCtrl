/*
 * spirecv.c
 *
 *  Created on: 20.07.2015
 *      Author: jan
 */

#include <spirecv.h>
#include "stm32f1xx_hal.h"

extern SPI_HandleTypeDef hspi1;


void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef *hspi)
{
    HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_9);
    if (hspi->Instance == hspi1.Instance)
    {

        HAL_SPI_Receive_IT(&hspi1, spi_recvbuf, SPI_RECSIZE);

    }
}
