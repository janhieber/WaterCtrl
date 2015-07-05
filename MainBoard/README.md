News: First PCBs are ordered!


# Progress
First PCBs arrived!

![PCB top](https://raw.githubusercontent.com/janhieber/WaterCtrl/master/MainBoard/docu/pcb_top1.jpg) | ![PCB bottom](https://raw.githubusercontent.com/janhieber/WaterCtrl/master/MainBoard/docu/pcb_bottom1.jpg)

# Info
This is the Eagle 6.5 schematic/layout for the main board.

When I'm working on schematics or layout I'll update the
[schematic.pdf](https://github.com/janhieber/WaterCtrl/raw/master/MainBoard/docu/schematic.pdf) and
[board.png](https://github.com/janhieber/WaterCtrl/raw/master/MainBoard/docu/board.png) files.

Current layout
![routed board](https://raw.githubusercontent.com/janhieber/WaterCtrl/master/MainBoard/docu/board.png)

This is the STM32 board I have chosen.
![stm32board](https://raw.githubusercontent.com/janhieber/WaterCtrl/master/MainBoard/docu/stm32board.jpg)

It has a low power ARM Cortex M3 CPU running at max 72MHz.  
Should be enough for a PWM and frequency measurement.  
It's very cheap (look on eBay) and has no useless peripherals.

# Order PCBs
For cheap PCBs you can order here: [http://dirtypcbs.com](http://dirtypcbs.com)

Because most people will [have to] order 5 or 10 pieces you can ask
me if someone has some PCBs left. If you have PCBs left, please tell me via Github Issue
or mail: mail at janhieber dot net


# Order parts
The parts are all from Reichelt (http://www.reichelt.de/) except the
STM32F1 CPU which I ordered on eBay.

Every part in the schematic has a attribute "R" with the Reichelt
part number as value.

To generate the BOM do the following:
 1. Open the schematic in Eagle
 2. File > Export > BOM
 3. Select "values" as list type

Now, Reichelt expects a CSV file with the following format:  
> DIODE GN;2  
> SMD 0805 100K;12

If you are working on Linux or Mac OSX you can execute this in a terminal:  
```bash
tail -n +2 BOM.csv | sed 's/\"//g' | awk -F ';' 'length($11)>0{print $11";"$1}' > BOM_reichelt.csv
```

*make sure you are in the directory where the BOM.csv is*

Otherwise use Libreoffice or Excel etc to generate the list, or well, add everything
manually to cart.

Now register at Reichelt and login.  
Click on your username on top and then "load cart" and import the CSV.


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
<<<<<<< HEAD
  - [ ] schematic
  - [ ] routed
  - [ ] tested
=======
  - [x] schematic
  - [x] routed
  - [ ] tested
>>>>>>> ba06be28f703d0df5e12109daff27424b3daf9bc
