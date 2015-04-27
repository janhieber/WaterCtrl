#include <stm32f0xx_conf.h>

#include <stdio.h>
#include <stdbool.h>
//#include "diag/Trace.h"

#if defined ( __GNUC__ )
	#define likely(x)		__builtin_expect(!!(x), 1)
	#define unlikely(x)		__builtin_expect(!!(x), 0)
#else
	#define likely(x)		x
	#define unlikely(x)		x
#endif

#define UNUSED(...)		(void)(__VA_ARGS__)

#define VERSION 1

// keep this file clean! only the main programm should be here
// every specific code should be other c files

#include <helpers.h>
#include <flash.h>
#include <moistureMeasure.h>
#include <motors.h>


void main(void)
{
	SystemCoreClockUpdate();
	if (SysTick_Config(SystemCoreClock / 1000)) { /* SysTick 1 mSec interrupts  */
		while(1);
	}

	initGPIOs();
	initUSART1();
	printf("\r\n\tWaterCtrl version %d\r\n", VERSION);
	printf("\tSystem clock: %luMHz\r\n\r\n", SystemCoreClock/1000000);

	// for WLAN
	initUSART2();
	initMotors();
	initMoistureMeasure();

	while (1)
	{
		GPIO_SetBits(GPIOC, GPIO_Pin_9);
		Delay(500);

		for (int channel=0;channel<8;channel++)
		{
			printf("Measured channel %d: %llu",channel, getMoisture(channel));
		}

		GPIO_ResetBits(GPIOC, GPIO_Pin_9);
		Delay(500);
	}

}



