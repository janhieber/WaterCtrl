# STM32 development with QtCreator



# Flashing
On the left side of QtCreator click *Projects*.  
Select *Run* in *STM32* Kit and click *Add* -> *Custom Executable*
to add the flash profile.
Enter the following:  
Executable: /usr/bin/openocd
Command line arguments: -s /usr/share/openocd/scripts -f interface/stlink-v2.cfg -f target/stm32f1x.cfg -c "reset_config none separate" -c "init" -c "reset halt" -c "flash write_image erase qtc_STM32_7c9f2a9c-debug/WaterCtrl-v1.qtc-STM32-7c9f2a9c.60b92938/WaterCtrl_v1.bin 0x08000000" -c "reset run" -c shutdown
Working diretory: %{buildDir}