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
        registrationCounter++;
    } else {

    }
    return retval;
}

void BrokerTask250ms()
{
    int params = 0;
    int runner = 1;
    char id = 0;
    char buf[SPI_XFER_SIZE];

    if (spiReceive(buf)) {
        LogUart(LogInfo, "broker: SPI messages was received");
//        do {
//            if (buf[runner] != 0)
//                params++;
//            else
//                break;
//        } while (1);

        id = buf[0];
        runner = 0;
        while (runner < MAX_NBR_OF_CLIENTS) {
            if (listIds[runner] == id ) {
                listFcts[runner](&id,params);
                break;
            }
            runner++;
        }
        snprintf(buf,SPI_XFER_SIZE,"%s","ACK");
        spiSend(buf[1],buf);
    }
}

/**
  * @}
  */
