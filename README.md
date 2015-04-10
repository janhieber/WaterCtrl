# WaterCtrl
Automatically pour your flowers

# About
This project is about schematics and software for a complete system to automatically
water your flowers. Yes, you can eventually go to holiday after years staying at home
because of your flowers.

Currently it is under development.
The measure boards are finished but not tested.
The main board is nearly finished, prototype coming soon.
Just startet at the firmwar development.

The idea is to measure the moisture of your flowser soil and water them when needed.
The watering is dong according to moisture or at a specific time intervall (for cactus).

The board has a display and buttons for direct configuration.
Later we use the WLAN interface to connect to a webserver running somewhere.
On the server you can also do configuration und observe th waterng.

# More detailes
Main board:
- ARM Cortex M0 (STM32F051)
- 6 motor controls up to 5A at 12V
- 8 sensor inputs
- ESP8266 WLAN adapter
- USB to UART bridge, yes you dont need a special converter
- big display
- 4 buttons

Measure:
The moisture measurement is done by a oscillating circuit that uses two big copper planes as capacitor in the soil. When the soil is wet the capacity is high and the frequency will be low.

Please look in the subfolder, especially in the MoistureMeasure 
and MainBoard folder are pictures and PDFs if the schematics and board.
