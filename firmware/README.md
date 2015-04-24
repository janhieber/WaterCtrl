This is the main firmware.

- [ ] Frequency measurement
- [ ] Play with ESP8266 connectivity


# Note
I use this folder as eclipse workspace.

# Dev environment
Install the following tools.
On Linux systems many tools are in your distro repo.
- Eclipse (eclipse)
- Eclipse CDT (eclipse-cdt)
- OpenOCD (openocd)
- STLink v2 (stlink)
- GNU ARM GCC embedded (arm-none-eabi-gcc, arm-none-eabi-gdb, arm-none-eabi-newlib, arm-none-eabi-binutils)

In Eclipse:
Help > Install new software > work with: Luna ....
In "Mobile and Device Development" select:
- C/C++ GCC Cross Compiler Support
- C/C++ GDB Hardware Debugging
- C/C++ Memoryy View Enhancements
and install them.

In Eclipse:
Help > Install new software > work with: Add new
name: GNU ARM Eclipse Plug-ins
location: http://gnuarmeclipse.sourceforge.net/updates
Select all and install.

Now switch your workspace to "firmware" folder of this project.
In menu: Window > Open Perspective > Packs
In middle window click on the two arrows with the tooltip: update the package definitions ....
When finished switch to "Devices" on the left and select
STMicroelectronics > STM32F0 Series
In middle view rightclick on latest version and install.
In status bar at the bottom you see the progress.

When finished switch to C/C++ perspective,
open project properties of "fw_v1" and go to
C/C++ Build > Settings > Cross ARM GNU Create Flash Image > General
Select "Raw Binary" as Output format.

No you should be able to build the project.
In external tools there is a flash entry.

For serial console, connect to TX/RX to PB6 and PB7.

