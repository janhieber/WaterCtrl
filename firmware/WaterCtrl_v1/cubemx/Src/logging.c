/** @file logging.c
 *  @brief Functions for logging messages and routing/filter them.
 *
 *  @author Jan Hieber <mail@janhieber.net>
 */

#include <logging.h>
#include <mxconstants.h>

LogDestination log_destination = LogDstSerConsole;

/*void log(LogLevel loglevel, char* msg){
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


}*/


void logSetDestination(LogDestination destination){
    log_destination = destination;
}
