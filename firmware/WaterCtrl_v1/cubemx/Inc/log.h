/** @file log.h
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

#ifndef LOGGING_H
#define LOGGING_H

#include <string.h>

typedef enum { LogError = 0x0, LogInfo = 0x1, LogDebug = 0x2 } LogLevel;

typedef enum {
  LogDstSerConsole = 0x0,
  LogDstRaspberryPi = 0x1,
  LogDstNoDestination = 0x2
} LogDestination;

/** @brief Do logging
 *  @param loglevel The loglevel of enum type LogLevel
 *  @param msg message to send as 0 terminated string
 */
void Log(LogLevel loglevel, char *msg);

/** @brief Set log desftination
 *  @param destination The log destination as enum type of LogDestination
 */
void logSetDestination(LogDestination destination);

#endif // LOGGING_H

/**
  * @}
  */
/**
  * @}
  */
