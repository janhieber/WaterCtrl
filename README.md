# WaterCtrl
Automatically pour your flowers

# About
This project is about schematics and the software.

Currently it is under development, i just startet with the source und the main board
is not ready yet.

The idea is to measure the moisture of your flowser soil and water them when needed.

# More detailes
Main:
The main board should use a STM32F0 CPU. Currently I'm using the Discovery board, maybe i'll make one myself later…

Measure:
The moisture measurement is done by a oscillating circuit that uses two big copper planes as capacitor in the soil. When the soil is wet the capacity is high and the frequency will be low.

