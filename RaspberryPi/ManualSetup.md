## Burn the image

First burn the Raspbian-wheezy on your SD card. The Raspberry documentation shows you how to do that. (see: [Official Documentation](https://www.raspberrypi.org/documentation/installation/installing-images/))

## Connect via serial cable

Because we are using the Model A+ version of the Raspberry Pi we only have one USB plug which we need for our WLAN stick. So we have to connect via the GPIO pins and a serial cable.

There is a good [Tutorial](https://learn.adafruit.com/downloads/pdf/adafruits-raspberry-pi-lesson-5-using-a-console-cable.pdf) on Adafruit how to do this.

After a succesful connection you should do your config. Please run `sudo raspi-config`

## Remove XServer

To remove the xserver and all contained packages: 

	apt-get remove --auto-remove --purge libx11-.*

Then install deborphan to get rid of orphaned files:

	sudo apt-get install deborphan

If you want to see what has been orphaned do this:

	deborphan -sz

Then remove all orphaned files:

	sudo apt-get remove --purge $(deborphan)

Finally do this to remove the unnecessary packages that are not orphaned:

	sudo apt-get autoremove

Final result: Headless and clean as a whistle


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
	
	
## Run the webserver as a daemon

Save the following script in a file called `/etc/init.d/webinterface`

	#!/bin/sh

	### BEGIN INIT INFO
	# Provides:          webinterface
	# Required-Start:    $remote_fs $syslog
	# Required-Stop:     $remote_fs $syslog
	# Default-Start:     2 3 4 5
	# Default-Stop:      0 1 6
	# Short-Description: Put a short description of the service here
	# Description:       Put a long description of the service here
	### END INIT INFO

	# Change the next 3 lines to suit where you install your script and what you want to call it
	DIR=/home/pi/WaterCtrl/WebIf/Webserver
	DAEMON=$DIR/runserver.py
	DAEMON_NAME=webinterface

	# Add any command line options for your daemon here
	DAEMON_OPTS=""

	# This next line determines what user the script runs as.
	# Root generally not recommended but necessary if you are using the Raspberry Pi GPIO from Python.
	DAEMON_USER=root

	# The process ID of the script when it runs is stored here:
	PIDFILE=/var/run/$DAEMON_NAME.pid

	. /lib/lsb/init-functions

	do_start () {
	    log_daemon_msg "Starting system $DAEMON_NAME daemon"
   		start-stop-daemon --start --background --chdir $DIR --pidfile $PIDFILE --make-pidfile --user $DAEMON_USER --chuid $DAEMON_USER --startas $DAEMON -- $DAEMON_OPTS
	    log_end_msg $?
	}
	do_stop () {
	    log_daemon_msg "Stopping system $DAEMON_NAME daemon"
	    start-stop-daemon --stop --pidfile $PIDFILE --retry 10
	    log_end_msg $?
	}

	case "$1" in

   	 start|stop)
   	     do_${1}
   	     ;;

   	 restart|reload|force-reload)
   	     do_stop
   	     do_start
   	     ;;

   	 status)
   	     status_of_proc "$DAEMON_NAME" "$DAEMON" && exit 0 || exit $?
   	     ;;

   	 *)
   	     echo "Usage: /etc/init.d/$DAEMON_NAME {start|stop|restart|status}"
   	     exit 1
   	     ;;

	esac
	exit 0	

Make the script executable `chmod +x /etc/init.d/webinterface`. Finally, update the daemons `sudo update-rc.d webinterface defaults`.

Now you can start the webserver via the command `service webinterface start`.

## Create tables for database

	python /home/pi/WaterCtrl/WebIf/Webserver/manage.py create_db

## Add sample data

	cd /home/pi/WaterCtrl/WebIf/Webserver/
	python manage.py seed_db --seedfile 'data/db_user.json'
	python manage.py seed_db --seedfile 'data/db_log.json'
	python manage.py seed_db --seedfile 'data/db_motor.json'
	python manage.py seed_db --seedfile 'data/db_sensor.json'
	python manage.py seed_db --seedfile 'data/db_plant.json'
	python manage.py seed_db --seedfile 'data/db_watering.json'
	
## Open browser

Finally your webinterface should be up and running on port 80. Open your browser and test it!




	






##TODO

The image should contain a Linux system, ready to run
with all stuff we need (control software, webinterface, database...).

The control algorithm and webinterface should be a copy of this git repo,
so we can easy update it.