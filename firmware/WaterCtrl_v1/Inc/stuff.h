/** @file stuff.h
 *  @brief some usefull stuff
 *
 *  @author Jan Hieber <mail@janhieber.net>
 */

/** @addtogroup Stuff
  * @{
  */

#ifndef STUFF_H
#define STUFF_H

#include <stm32f1xx_hal.h>

void procAliveTicker(void const * argument);
void procWatchdog(void const * argument);


#endif // STUFF_H

/**
  * @}
  */
