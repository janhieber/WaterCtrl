# Info
This is the Eagle 6.x schematic/board for the main board.

When I'm working on the schematics I'll update the
'schematic.pdf', 'board.pdf' and 'board.png' files.
So you can watch the progress:

https://github.com/janhieber/WaterCtrl/raw/master/MainBoard/schematic.pdf

https://github.com/janhieber/WaterCtrl/raw/master/MainBoard/board.pdf

![routed board](https://raw.githubusercontent.com/janhieber/WaterCtrl/master/MainBoard/board.png)

# Order PCBs
To order PCBs you can simply send the Eagle files or generate the Gerber
files and send it to your board house.
For cheap PCBs you can order here:
http://dirtypcbs.com/
or here:
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

Then for each part the script tells you the quantity and opens a browser window.
Simply click the first result, input quantity and add to cart, press enter key in 
script to continue with next...

In OSX you may have to replace the 'xdg-open' call in the script with 'firefox' or
something else... I have no Mac to test this
 
# ToDo
- [ ] Circuit for the moisture measurement (connectors, demultiplexer)
  - [x] schematic
  - [x] routed
  - [ ] tested
- [ ] Circuit for the water pump controlling (FETs ...)
  - [x] schematic
  - [x] routed
  - [ ] tested
- [ ] Place ESP8266 module
  - [x] schematic
  - [x] routed
- [ ] Power supply circuit for 3.3V
  - [x] schematic
  - [x] routed
  - [ ] tested
- [ ] Wiring up the STM32F0 (Cs, crystal, etc...)
  - [x] schematic
  - [x] routed
  - [ ] tested