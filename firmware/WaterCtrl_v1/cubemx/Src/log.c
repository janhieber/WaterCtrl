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

#include <stdarg.h>

#include <log.h>
#include <mxconstants.h>
#include <spicomm.h>

static LogDestination log_destination = LogDstSerConsole | LogDstRaspberryPi;
static LogLevel log_filter = LogDebug | LogError | LogInfo;


void Log(LogLevel loglevel, const char* format, ...) {
  if (!(loglevel & log_filter)) return;

  char msg[STR_MAX_SIZE];
  va_list args;
  va_start( args, format );
  vsprintf( msg, format, args );
  va_end( args );

  if (log_destination & LogDstSerConsole) {
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
      default:
        break;
    }
  }

  if (log_destination & LogDstRaspberryPi) {
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
      default:
        break;
    }
  }
}

void LogUart(LogLevel loglevel, const char* format, ...) {
  if (!(loglevel & log_filter)) return;

  char msg[STR_MAX_SIZE];
  va_list args;
  va_start( args, format );
  vsprintf( msg, format, args );
  va_end( args );

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
    default:
      break;
  }
}


void logSetDestination(LogDestination destination) {
  log_destination = destination;
}

void logSetFilter(LogLevel filter) { log_filter = filter; }

/**
  * @}
  */
/**
  * @}
  */
