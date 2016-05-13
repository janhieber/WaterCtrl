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

#include <usart.h>
#include <stdint.h>
#include <stdbool.h>

/**
 * @brief System alive indicator for LED
 */
void procAliveTicker(void const * argument) {
	uint32_t wakeTime = osKernelSysTick();
	PROCRUNNING;

	while (true) {
		osDelayUntil(&wakeTime, 500);
		HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
	}
}


/**
 * Process for resetting watchdog
 */
void procWatchdog(void const * argument) {
#ifdef USEWATCHDOG
	uint32_t wakeTime = osKernelSysTick();

	HAL_IWDG_Start(&hiwdg);


	PROCRUNNING;
	while (true) {
		HAL_IWDG_Refresh(&hiwdg);
		osDelayUntil(&wakeTime, 1000);
	}
#else
    while(true)
    	osThreadSuspend(NULL);
#endif
}


/**
 * Process for boot sequence
 */
void procBoot(void const * argument) {
	uint32_t wakeTime = osKernelSysTick();
	UNUSED(wakeTime);

	PROCRUNNING;

	// check if watchdog resetted us the last time
	if (__HAL_RCC_GET_FLAG(RCC_FLAG_IWDGRST) != RESET) {
		__HAL_RCC_CLEAR_RESET_FLAGS();
		EE("system resumed from watchdog reset!");
	}



	while (true) {
		osThreadSuspend(NULL);
	}
}



void putc_(char c) {
	HAL_UART_Transmit(&huart1, (uint8_t *) &c, 1, 100);
}




/**
  * @}
  */
