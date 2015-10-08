/* ---------------------------------------------------------------------------
** This software is in the public domain, furnished "as is", without technical
** support, and with no warranty, express or implied, as to its usefulness for
** any purpose.
**
** logging.h
** Functions for logging messages and routing/filter them.
**
** Author: Jan Hieber <mail@janhieber.net>
** -------------------------------------------------------------------------*/

#ifndef LOGGING_H
#define LOGGING_H

// when changing this, also change in printf.c!
#define UART_MAX_SIZE 128

typedef enum {
    LogError = 0x0,
    LogInfo = 0x1,
    LogDebug = 0x2
} LogLevel;


typedef enum {
    LogDstSerConsole = 0x0,
    LogDstRaspberryPi = 0x1,
    LogDstNoDestination = 0x2
} LogDestination;

void log(LogLevel loglevel, char* msg);

void logSetDestination(LogDestination destination);



#endif // LOGGING_H
