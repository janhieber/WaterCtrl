


extern volatile uint32_t msTicks;

void initGPIOs(void);

void initUSART1(void);
void USART_puts(USART_TypeDef* USARTx, volatile char *s);
void USART_putc(USART_TypeDef* USARTx, char s);
//void USART1_IRQHandler(void);

void Delay(uint32_t msec);
