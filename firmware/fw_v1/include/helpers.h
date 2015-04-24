


#define UART1_BAUD 9600
#define UART2_BAUD 9600

extern volatile uint32_t msTicks;

void initGPIOs(void);

void initUSART1(void);
void initUSART2(void);

void USART_puts(USART_TypeDef* USARTx, volatile char *s);
void USART_puts_s(USART_TypeDef* USARTx, volatile char *s, int size);
void USART_putc(USART_TypeDef* USARTx, char s);


void Delay(uint32_t msec);
