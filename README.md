WaterCtrl - Automatically water your flowers

# About
This project is a complete system to automatically
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

The board will have a connector for a Raspberry Pi
which takes care of all high level functions like:
WLAN/LAN connection, webinterface, watering algorithm, weather station... 

The STM32 CPU will take care of the low level functions like:
watchdog for the rPi, PWM motor control, frequency measurement.

# Design
This is my idea of the device.

![Design](https://cdn.rawgit.com/janhieber/WaterCtrl/master/docu/Design.png)

And this is the hardware design.

![Design HW](https://cdn.rawgit.com/janhieber/WaterCtrl/master/docu/Design%20Hardware.png)


# Technical
Measure:
The moisture measurement is done by a oscillating circuit that uses two
big copper planes as capacitor in the soil.
When the soil is wet the capacity is high and the frequency will be low.

# More detailes
Please look in the subfolders for more informations,
nice pictures and other stuff.

# My future plans
I have some nice ideas, let me know what you think about this:

- The rPi will become the central control device with a radio module.
- Small sensor boards with a radio can interface a few sensors.
- Small motor control boards with a radio can control a few motors.
- The rPi, sensor and motor boards can be connected with wires when you don't want/need
  radio.
- Multiple sensor and motor boards can be used with one rPi. This way
  you can scale the system like you need it.

This will allow a complete modular system with most low level functions outsourced on
smaller modules and less long wires laying around.