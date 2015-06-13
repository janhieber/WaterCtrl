News: Redesign is in progress now.  
- Rasberry Pi will be on the top side, but flipped (top down).  
  Soldered with [SMD female headers](http://www.voelkner.de/products/478859/Fischer-Elektronik-SMD-Buchsenleiste-RM-2-54-mm-Low-Profile-Pole-2-x-20-BL-LP-6-SMD-40-Z-Inhalt.html)  
- STM32F103C8T6 mini dev board will be just below the rPi.  
  Soldered with [SMD pin headers](http://www.voelkner.de/products/297556/Stiftleisten-in-SMD-Technik-RM-2-54-mm-gewinkelt-SL-LP-5...-Pole-1-x-20-SL-LP-5-SMD-051-20-Z-Fisc.html)  

The SMD headers allow routing of wires on bottom side of the PCB.

# Info
This is the Eagle 6.5 schematic/layout for the main board.

When I'm working on schematics or layout I'll update the
[schematic.pdf](https://github.com/janhieber/WaterCtrl/raw/master/MainBoard/docu/schematic.pdf) and
[board.pdf](https://github.com/janhieber/WaterCtrl/raw/master/MainBoard/docu/board.pdf) files.

Current layout
![routed board](https://raw.githubusercontent.com/janhieber/WaterCtrl/master/MainBoard/docu/board.png)

This is the STM32 board I have chosen.
![stm32board](https://raw.githubusercontent.com/janhieber/WaterCtrl/master/MainBoard/docu/stm32board.jpg)

It has a low power ARM Cortex M1 CPU running at max 72MHz.  
Should be enough for a PWM and frequency measurement.  
It's very cheap (look on eBay) and has no useless peripherals.

# Order PCBs
For cheap PCBs you can order here:
http://www.elecrow.com/services-pcb-prototyping-c-73_116.html

Because most people will [have to] order 5 or 10 pieces you can ask
me if someone as some PCBs left. If you have PCBs left, please tell me via Github
or mail: mail at janhieber dot net

# Order parts
The parts are all from Reichelt (http://www.reichelt.de/) except the
STM32F0 CPU which I ordered on eBay.

Every part in the schematic has a attribute "R" with the Reichelt
part number as value.

To generate a BOM do the following:
 1. Open the schematic in Eagle
 2. File > Export > BOM    or    Datei > Exportieren > BOM
 3. I use "values" or "Werte" as list type.
 4. Maybe Reichelt accepts this list if you ask nice :)

If you are using Linux or Mac OSX you can use my order script, its called "orderBOM.sh"
in this folder.

Simply go into this folder with your terminal and execute it: ./orderBOM.sh

# ToDo
- [ ] Circuit for the moisture measurement (connectors, demultiplexer)
  - [x] schematic
  - [x] routed
  - [ ] tested
- [ ] Circuit for the water pump controlling (FETs ...)
  - [x] schematic
  - [x] routed
  - [ ] tested
- [ ] Power supply circuit for 5V
  - [x] schematic
  - [x] routed
  - [ ] tested
- [ ] Wiring up the STM32 and rPi
  - [ ] schematic
  - [ ] routed
  - [ ] tested