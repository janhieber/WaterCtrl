WaterCtrl - Automatically water your flowers

# BREAKING NEWS
I'll completely redesign the main board.
This is because we wan't to connect the rPi and the STM32
via CAN bus. But the CPU i have chosen STM32F051 does not have CAN.
I selected a better CPU STM32F072RBT6 that completely fits our needs.
Luckily there is a brand new Discovery board with this CPU,
with this board we don't have to solder the CPU on our board.

So the new board will have two headers, one for the Discovery board
and one for the Raspberry Pi.

This way we have a ver modular design und less trouble with soldering LQFP packages.


# About
This project is about a complete system to automatically
water your flowers. Yes, you can eventually go to holiday after years staying at home
because of your flowers.

Currently it is under development.
- The measure boards are finished.
- The main board is is in prototype stage.
- The Webinterface is nearly finished.
- The firmware development was just startet.

The idea is to measure the moisture of your flowser soil and
water them when needed. The watering is done according to moisture
or at a specific time intervall (for cactus).

The board will have a slot for a Raspberry Pi (the old display
and buttons will be removed soon).
The STM32 CPU on the main board is for low level functions and
watchdog for the rPi.
The rPi will run the main control app, serve the Webinterface,
do logging to database, delegate low level functions to STM32.

# Design
This is my idea of the device. Can be changed at any time.
![Design](https://raw.githubusercontent.com/janhieber/WaterCtrl/master/doc/Design.png)

# More detailes
Main board:
- ARM Cortex M0 (STM32F051)
- 3V power supply
- 6 motor controls up to 5A at 12V
- 8 sensor inputs
- USB to UART bridge, yes you dont need a special converter
- I2C or SPI to rPi
- bright error indication LED
- rPi connector

# Technical
Measure:
The moisture measurement is done by a oscillating circuit that uses two
big copper planes as capacitor in the soil.
When the soil is wet the capacity is high and the frequency will be low.

# More detailes
Please look in the subfolders for more informations,
nice pictures and other stuff.