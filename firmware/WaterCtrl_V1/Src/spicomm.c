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

#include <spicomm.h>
#include "stm32f1xx_hal.h"
#include <stdlib.h>

SPI_HandleTypeDef hspi1;

typedef struct SpiMessage SpiMessage;
struct SpiMessage {
    SpiMessage *prev;
    SpiMessage *next;
    char *msg;
};

SpiMessage *sendQueue;
SpiMessage *recvQueue;

void spiQueueInit(void) {
    sendQueue = NULL;
    recvQueue = NULL;
}

SpiMessage* spiNewMessage(void) {
    SpiMessage *spimsg =
            (struct SpiMessage*)malloc(sizeof(struct SpiMessage));
    spimsg->prev = NULL;
    spimsg->next = NULL;
    spimsg->msg = NULL;
    return spimsg;
}

void spiDelMessage(SpiMessage *spimsg) {
    //free(spimsg->msg); // we free the msg where its used
    free(spimsg);
}

//void spiQueueAppend(SpiQueue spiqueue, SpiMessage *spimsg) {
//    SpiMessage *queue = (spiqueue == SpiQueueSend) ? sendQueue : recvQueue;
//    if (queue == NULL)
//        queue = spimsg;
//    else {
//        while (queue->next != NULL)
//            queue = queue->next;
//        queue->next = spimsg;
//        spimsg->prev = queue;
//    }
//}

void spiQueuePrepend(SpiQueue spiqueue, SpiMessage *spimsg) {
    SpiMessage *queue = (spiqueue == SpiQueueSend) ? sendQueue : recvQueue;
    if (queue == NULL)
        queue = spimsg;
    else {
        SpiMessage *tmp = queue;
        queue = spimsg;
        queue->next = tmp;
        tmp->prev = queue;
    }
}

void spiQueuePush(SpiQueue spiqueue, char *msg) {
    SpiMessage *spimsg = spiNewMessage();
    spimsg->msg = msg;
    spiQueuePrepend(spiqueue, spimsg);
}

SpiMessage* spiQueuePopFront(SpiQueue spiqueue) {
    SpiMessage *queue = (spiqueue == SpiQueueSend) ? sendQueue : recvQueue;
    if (queue == NULL)
        return NULL;
    SpiMessage *tmp = queue;
    queue = queue->next;
    return tmp;
}


char* spiQueuePop(SpiQueue spiqueue) {
    SpiMessage *spimsg = spiQueuePopFront(spiqueue);
    if (spiqueue == NULL)
        return NULL;
    char *msg = spimsg->msg;
    spiDelMessage(spimsg);
    return msg;
}


//char* spiQueuePopBack(SpiQueue spiqueue) {
//    SpiMessage *queue = (spiqueue == SpiQueueSend) ? sendQueue : recvQueue;
//    if (queue == NULL)
//        return NULL;
//    SpiMessage *tmp = queue;
//    queue = queue->next;
//    char *msg = tmp->msg;
//    spiDelMessage(tmp);
//    return msg;
//}

void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef *hspi) {
    HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_9);
    if (hspi->Instance == hspi1.Instance) {

        HAL_SPI_Receive_IT(&hspi1, spi_recvbuf, SPI_RECSIZE);

    }
}
