# Raspberry Pi
This is for the Raspberry Pi only.

## Burn the image

First burn the Raspbian-wheezy on your SD card. The Raspberry documentation shows you how to do that. (see: [Official Documentation](https://www.raspberrypi.org/documentation/installation/installing-images/))

## Connect via serial cable

Because we are using the Model A+ version of the Raspberry Pi we only have one USB plug which we need for our WLAN stick. So we have to connect via the GPIO pins and a serial cable.

There is a good [Tutorial](https://learn.adafruit.com/downloads/pdf/adafruits-raspberry-pi-lesson-5-using-a-console-cable.pdf) on Adafruit how to do this.

After a succesful connection you should do your config. Please run `sudo raspi-config`


## WiFi

To have access to the webinterface you must provide an ethernet accesss.

We used a simple LogiLink WLAN Stick. In this section we will explain how to install and configure your WiFi.

First, we need to check wether our stick is recognised or not. Type in `lsusb`. You should see some output with "WLAN" or "Wireless".

To scan for WiFi networks, use the command `sudo iwlist wlan0 scan`.

  - `ESSID:"testing"`. This is the name of the WiFi network. 
  - `IE: IEEE 802.11i/WPA2 Version 1`. This is the authentication used for the WiFi network.

Then open the network config file:

    sudo nano /etc/network/interfaces
    
We want to use a static ip. This is better for our webinterface which we will use later.

	auto lo 
	iface lo inet loopback 
	iface eth0 inet dhcp

	auto wlan0 
	allow-hotplug wlan0 
	iface wlan0 inet static
	address 192.168.0.230
	netmask 255.255.255.0
	gateway 192.168.0.1
	dns-nameservers 192.168.0.1
	wpa-ap-scan 1 
	wpa-scan-ssid 1 
	wpa-ssid "{WLANSSID}" 
	wpa-psk "{WLANPASSWORD}"

You can verify if it has successfully connected using `ifconfig wlan0`. Perhaps you have to reboot your system `sudo reboot`.

## Install GIT

To clone our repository we have to install GIT:

	sudo apt-get install git 
	
Then clone the repository:

	git clone https://github.com/janhieber/WaterCtrl.git
	
## Install database

We will use MySQL. Install MySSQL:

	sudo apt-get install mysql-server
	
Then you will be prompted to type in your root passwords.

### Create a database

	mysql -u root -p
	CREATE DATABASE waterctrl;
	CREATE USER 'waterctrl_user'@'localhost' IDENTIFIED BY 'W@t3rC!rl';
	GRANT ALL ON waterctrl.* TO 'waterctrl_user'@'localhost';
	FLUSH PRIVILEGES;

## Install required dependencies

We will need PIP for our webinterface dependencies:

	sudo apt-get install python-pip libmysqlclient-dev python-dev
	
Then go to the webinterace folder
	
	cd /home/pi/WaterCtrl/WebIf/Webserver
	
Install the dependencies:

	sudo pip install -r requirements.txt
	
## Run the webserver



	






##TODO

The image should contain a Linux system, ready to run
with all stuff we need (control software, webinterface, database...).

The control algorithm and webinterface should be a copy of this git repo,
so we can easy update it.