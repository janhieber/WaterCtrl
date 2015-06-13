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