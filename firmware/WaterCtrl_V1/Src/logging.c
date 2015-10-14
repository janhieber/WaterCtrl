/* ---------------------------------------------------------------------------
** This software is in the public domain, furnished "as is", without technical
** support, and with no warranty, express or implied, as to its usefulness for
** any purpose.
**
** logging.c
** *see header file*
**
** Author: Jan Hieber <mail@janhieber.net>
** -------------------------------------------------------------------------*/

#include <logging.h>

LogDestination log_destination = LogDstSerConsole;

void log(LogLevel loglevel, char* msg){
    if (strlen(msg) > STR_MAX_SIZE) return;

    switch (log_destination) {
    case LogDstSerConsole:
        switch (loglevel) {
        case LogError:
            printf("EE: %s", msg);
            break;
        case LogInfo:
            printf("II: %s", msg);
            break;
        case LogDebug:
            printf("DD: %s", msg);
            break;
        }
        break;
    case LogDstRaspberryPi:
        switch (loglevel) {
        case LogError:
            // add to SPI queue
            //printf("EE: %s", msg);
            break;
        case LogInfo:
            //printf("II: %s", msg);
            break;
        case LogDebug:
            //printf("DD: %s", msg);
            break;
        }
        break;
    case LogDstNoDestination:
    default:
        break;
    }


}


void logSetDestination(LogDestination destination){
    log_destination = destination;
}
