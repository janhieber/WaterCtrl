# Flash current firmware
We copy some releases to the *bin* folder.
Download the latest and flash as follows:

To flash these firmares with Linux or OSX use the flash script:
```bash
./flashfw.sh <fwfile.bin>
```

When using Windows, download the [ST-Link utility](http://www.st.com/web/en/catalog/tools/PF258168).  
In the menu you can select a file to flash.



# Dev environment
Download and install System Workbench from [openstm32](http://www.openstm32.org/HomePage)

or:  
Integrate System Workbench in your existing Eclipse. Read on the [openstm32](http://www.openstm32.org/HomePage)
website how to do this. This is the better solution as the shipped Eclipse is
a bit out of date.


Open System Workbench/Eclipse and create a workspace or use your existing.  
Do _not_ use a workspace withing this git repo!

Click *File* > *Import* > *Existing Project into Workspace*  
Browse to the git repo and select *firmware/WaterCtrl_v1* folder.

Uncheck *Copy projects into workspace* and click *Finish*.  
Now you have your project in the project explorer and can work with it
and sync your changes to git.

Modify the last line in the file *WaterCtrl_v1.cfg*.
Should be: *reset_config none separate*

Click the green play/run button to flash the software. Debug also works.

Have fun!


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


