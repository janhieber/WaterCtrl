/**
*
*
* @file Broker API
*
*  @description Broker will forward SPI messages to registered clients.
*
*  Registration along with message identifier to get certain messages will
*  reduce the cpu load by less function calls.
*/

/** @addtogroup Broker
  * @{
  */
#ifndef BROKER_H
#define BROKER_H

#define BRK_ERR_NO_ERR (uint8_t)0x00u
#define BRK_ERR_MSG_PARAM_INVALID (uint8_t)0x01u
#define BRK_ERR_MOTOR_START_FAILED (uint8_t)0x02u

#define MAX_NBR_OF_CLIENTS 16

typedef void (message)(char *,uint8_t);
bool registerMessage(uint8_t,message*);
bool deregisterMessage(uint8_t);

void BrokerTask250ms();

#endif // BROKER_H

/**
  * @}
  */
