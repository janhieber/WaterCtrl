/*
 * helpers.c
 *
 *  Created on: 30.03.2015
 *      Author: jan
 */
#include <stm32f0xx_conf.h>

#include <helpers.h>

volatile uint32_t msTicks = 0;

void initSystem(void) {
	SysTick_Config (SystemCoreClock / 1000u);
}

void initGPIOs(void) {
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
	GPIO_InitTypeDef gpioStructure;

	// Green LED
	GPIO_StructInit(&gpioStructure);
	gpioStructure.GPIO_Pin = GPIO_Pin_9;
	gpioStructure.GPIO_Mode = GPIO_Mode_OUT;
	gpioStructure.GPIO_Speed = GPIO_Speed_2MHz;
	gpioStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	gpioStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_Init(GPIOC, &gpioStructure);

}

void initUSART1(void) {
	USART_InitTypeDef USART_InitStructure = {0,};
	GPIO_InitTypeDef GPIO_InitStructure = {0,};
	NVIC_InitTypeDef NVIC_InitStructure = {0,};

	// configuring clock sources for USART1
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

	GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_0);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_0);

	/* Configure USART1 pins:  Rx and Tx ----------------------------*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	/* Enable USART1 IRQ */
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl =
			USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);

	// enabling USART1
	USART_Cmd(USART1, ENABLE);

	// enable incoming data interrupt
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
}

void USART_puts(USART_TypeDef* USARTx, volatile char *s) {
	while (*s) {
		while (USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET)
			;
		USART_SendData(USARTx, *s);
		*s++;
	}
}

void USART_putc(USART_TypeDef* USARTx, char s) {
	while (USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET)
		;
	USART_SendData(USARTx, s);
}

void Delay(uint32_t msec) {
	uint32_t currentTicks = msTicks;
	while ((msTicks - currentTicks) < msec);
}

void USART1_IRQHandler(void) {
	uint8_t data = 0x00;

	if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) {
		data = USART_ReceiveData(USART1);

		USART_puts(USART1, "Received character: ");
		USART_putc(USART1, data);
	}
}

void SysTick_Handler(void) {
	msTicks++;
}
