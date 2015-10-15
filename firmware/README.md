# Firmware
This is the firmware for the STM32.



# Flash current firmware
We copy the current firmware into this folder, see the .bin files.

To flash these firmares with Linux or OSX use the flash script:
```bash
./flashfw.sh <fwfile.bin>
```

When using Windows, download the [ST-Link utility](http://www.st.com/web/en/catalog/tools/PF258168).  
In the menu you can select a file to flash.



# Dev environment
We use [Coocox](http://www.coocox.org/software.html) for development with ARM GCC embedded.

*Before we used Eclipse with GNU ARM Eclipse plugins, but the Eclipse workspace
and project are many files which are difficult to handle with git and sync with others.
Also the setup was complicated and the configuration for flashing and debugging are complicated and vary from
system to system.  
This is why we now use Coocox, a little sad about the fact that it does not work
under Linux (for now) but so shall it be.*

You will also need [ARM GCC embedded](https://launchpad.net/gcc-arm-embedded/+download) toolchain.

If you want to change things at the lowest level you will also need the initialisation
code generator [STM32CubeMX](http://www.st.com/web/catalog/tools/FM147/CL1794/SC961/SS1743/PF259242?sc=microxplorer).




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


# How is the project organised?
First, the project is a default Coocox project. You will find a subfolder named
*cubemx*, in this folder is the STM32CubeMX project and the generated source code.  
Most of the code and the generated project file are of no use to us, we simply ignore them.  
For the files we use, we create a *group* in the Coocox project view and add the files.  
This way we can simply regenerate the code without any problems or conflict with Coocox.  
Remember: Some files are not added to the project, for example the HAL *.c* files we dont use,
like *cubemx\Drivers\STM32F1xx_HAL_Driver\Src\stm32f1xx_hal_adc.c*.
The *.h* files are all in the project, so if you get *implicit declaration of ...* warnings,
add the missing *.c* files to the project group.

