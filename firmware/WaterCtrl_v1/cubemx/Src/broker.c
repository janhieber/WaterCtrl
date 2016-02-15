#include <stdint.h>
#include <stdbool.h>

#include <log.h>
#include <spicomm.h>
#include <broker.h>
/** @addtogroup Broker
  * @{
  */

message * listFcts[MAX_NBR_OF_CLIENTS];
uint8_t listIds[MAX_NBR_OF_CLIENTS];
uint8_t registrationCounter;

bool registerMessage(uint8_t id, message* fct)
{
    bool retval = false;
    if (registrationCounter < MAX_NBR_OF_CLIENTS) {
        listFcts[registrationCounter] = fct;
        listIds[registrationCounter] = id;
        retval = true;
    } else {

    }
    return retval;
}

void BrokerTask250ms()
{
    char buf[SPI_XFER_SIZE];
    if (spiReceive(buf)) {
        LogUart(LogInfo, "SPI messages was received");

        do {
            int runner = 0;
            while (runner < MAX_NBR_OF_CLIENTS) {
                if (listIds[runner] == buf[0] ) {
                    listFcts[runner](&buf[1],buf[0]);
                    break;
                }
            }
        }while(0);
    }
}

/**
  * @}
  */
