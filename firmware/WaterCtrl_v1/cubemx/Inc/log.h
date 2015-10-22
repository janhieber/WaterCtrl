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

/*! these are the log levels for message */
typedef enum { LogError = 0x00, LogInfo = 0x01, LogDebug = 0x04 } LogLevel;

/*! these are the possible log destinations */
typedef enum {
  LogDstSerConsole = 0x01,
  LogDstRaspberryPi = 0x02
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

/** @brief Set log filter
 *  @param filter The log filter as enum type of LogDestination
 */
void logSetFilter(LogLevel filter);

#endif  // LOGGING_H

/**
  * @}
  */
/**
  * @}
  */
