# Firmware
This is the firmware for the STM32.

There should be low level functionality only.  
*Every high level control is done by the rPi.*
- [ ] Sensor measurement
- [ ] motor control
- [ ] UART logging/debugging
- [ ] SPI for rPi communication
- [ ] watchdog for rPi


# Note
I use this folder as eclipse workspace.

# Dev environment

Read [Dev Environment.pdf](https://raw.githubusercontent.com/janhieber/WaterCtrl/master/firmware/docu/Dev%20Environment.pdf)
to setup Eclipse for the STM32.

Now switch your workspace to "firmware" folder of this project.  

No you should be able to build the project.  
In external tools there is a flash entry.

# CPU Pins

|Position|Name           |Type  |Signal        |Label     |
|--------|---------------|------|--------------|----------|
|2       |PC13-TAMPER-RTC|Output|GPIO_Output   |RPI_NRST  |
|3       |PC14-OSC32_IN  |Output|GPIO_Output   |PWM_A0    |
|4       |PC15-OSC32_OUT |Output|GPIO_Output   |PWM_A1    |
|5       |PD0-OSC_IN     |I/O   |RCC_OSC_IN    |          |
|6       |PD1-OSC_OUT    |I/O   |RCC_OSC_OUT   |          |
|10      |PA0-WKUP       |Output|GPIO_Output   |PWM_A2    |
|11      |PA1            |I/O   |TIM2_CH2      |PWM       |
|15      |PA5            |I/O   |SPI1_SCK      |          |
|16      |PA6            |I/O   |SPI1_MISO     |          |
|17      |PA7            |I/O   |SPI1_MOSI     |          |
|18      |PB0            |I/O   |TIM3_CH3      |FREQ      |
|19      |PB1            |Output|GPIO_Output   |SEN_A0    |
|21      |PB10           |Output|GPIO_Output   |SEN_A1    |
|22      |PB11           |Output|GPIO_Output   |SEN_A2    |
|28      |PB15           |Output|GPIO_Output   |SEN_ENABLE|
|30      |PA9            |I/O   |USART1_TX     |          |
|31      |PA10           |I/O   |USART1_RX     |          |
|34      |PA13           |I/O   |SYS_JTMS-SWDIO|          |
|37      |PA14           |I/O   |SYS_JTCK-SWCLK|          |
|42      |PB6            |I/O   |TIM4_CH1      |          |
|46      |PB9            |Output|GPIO_Output   |LED_ERR   |
