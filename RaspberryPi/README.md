# Raspberry Pi
This is for the Raspberry Pi only.

There will be an SDCard image and other things...


## WiFi

To have access to the webinterface you must provide an ethernet accesss.

We used a simple WLAN Stick (...). In this section we will explain how to install and configure your WiFi.

There is a good tutorial on how to configure your WiFi with Raspbian (see: [Setting WiFi up via the command line](https://www.raspberrypi.org/documentation/configuration/wireless/wireless-cli.md))

1. Getting WiFi network details

To scan for WiFi networks, use the command `sudo iwlist wlan0 scan`.

  - `ESSID:"testing"`. This is the name of the WiFi network. 
  - `IE: IEEE 802.11i/WPA2 Version 1`. This is the authentication used for the WiFi network.
  
2. Adding the network details to the Raspberry Pi

Open the `wpa-supplicant` configuration file in nano:

    sudo nano /etc/wpa_supplicant/wpa_supplicant.conf

Go to the bottom of the file and add the following:

    network={
    	ssid="The_ESSID_from_earlier"
    	psk="Your_wifi_password"
    }

Save the file by pressing ctrl+x then y.

You can verify if it has successfully connected using `ifconfig wlan0`. 

3. klkj
4. lkjlkj
5. lkj



##TODO

The image should contain a Linux system, ready to run
with all stuff we need (control software, webinterface, database...).

The control algorithm and webinterface should be a copy of this git repo,
so we can easy update it.