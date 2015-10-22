/** @file log.c
 *  @brief Functions for logging messages and routing/filter them.
 *
 *  @author Jan Hieber <mail@janhieber.net>
 */

/** @addtogroup Communication
  * @{
  */

/** @addtogroup Logging
  * @{
  */

#include <log.h>
#include <mxconstants.h>

static LogDestination log_destination = LogDstSerConsole;

void Log(LogLevel loglevel, char *msg) {
  if (strlen(msg) > STR_MAX_SIZE)
    return;

  switch (log_destination) {
  case LogDstSerConsole:
    switch (loglevel) {
    case LogError:
      printf("EE: %s\r\n", msg);
      break;
    case LogInfo:
      printf("II: %s\r\n", msg);
      break;
    case LogDebug:
      printf("DD: %s\r\n", msg);
      break;
    }
    break;
  case LogDstRaspberryPi:
    switch (loglevel) {
    case LogError:
      spiSend(0x01, msg);
      break;
    case LogInfo:
      spiSend(0x02, msg);
      break;
    case LogDebug:
      spiSend(0x03, msg);
      break;
    }
    break;
  case LogDstNoDestination:
  default:
    break;
  }
}

void logSetDestination(LogDestination destination) {
  log_destination = destination;
}


/**
  * @}
  */
/**
  * @}
  */
