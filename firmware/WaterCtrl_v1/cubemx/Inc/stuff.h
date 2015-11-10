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

void AliveTicker(void);
void WatchdogReset(IWDG_HandleTypeDef *hiwdg);
void WatchdogStart(IWDG_HandleTypeDef *hiwdg);



#endif // STUFF_H

/**
  * @}
  */
