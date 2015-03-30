#include <stm32f0xx_conf.h>

#include <stdio.h>
#include "diag/Trace.h"

#include <helpers.h>



void main(void)
{
	SystemCoreClockUpdate();
	if (SysTick_Config(SystemCoreClock / 1000)) { /* SysTick 1 mSec interrupts  */
		while(1);
	}

	trace_puts("Hello ARM World!");
	trace_printf("System clock: %uHz\n", SystemCoreClock);


	initGPIOs();
	initUSART1();

	while (1)
	{
		GPIO_SetBits(GPIOC, GPIO_Pin_9);
		Delay(500);

		GPIO_ResetBits(GPIOC, GPIO_Pin_9);
		Delay(500);
	}

}



