/** @file stuff.c
 *  @brief some usefull stuff
 *
 *  @author Jan Hieber <mail@janhieber.net>
 */

/** @addtogroup Stuff
  * @{
  */

#include "stm32f1xx_hal.h"

#include <mxconstants.h>

/**
 * @brief System alive indicator for LED
 */
void AliveTicker(void) {
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
  HAL_Delay(50);
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
}

/**
 * @brief Reset watchdog
 */
void WatchdogReset(IWDG_HandleTypeDef *hiwdg) {
#ifdef USEWATCHDOG
  HAL_IWDG_Refresh(hiwdg);
#endif
}

/**
 * @brief Start watchdog
 */
void WatchdogStart(IWDG_HandleTypeDef *hiwdg) {
#ifdef USEWATCHDOG
  IWDG_ENABLE_WRITE_ACCESS(hiwdg);
  __HAL_IWDG_START(hiwdg);
#endif
}

/**
  * @}
  */
