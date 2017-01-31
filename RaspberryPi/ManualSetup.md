If you don't want to use our image and setup everything yourself, follow this guide.


## Get Raspbian on SD card
[Official Documentation](https://www.raspberrypi.org/documentation/installation/installing-images/)


## Connect via serial cable
Connect to the Raspberry Pi, we prefer serial cable (you can use SSH later when networking is up).  
There is a good [Tutorial](https://learn.adafruit.com/downloads/pdf/adafruits-raspberry-pi-lesson-5-using-a-console-cable.pdf) on Adafruit how to do this.


## Remove XServer
We prefer a lightweight image, so remove everything we don't need.  
If you want to use HDMI display, you can skip this.

To remove the xserver and all contained packages:
```shell
sudo apt-get remove --auto-remove --purge libx11-.*
```

Then install deborphan to get rid of orphaned files:
```shell
sudo apt-get install deborphan
```

If you want to see what has been orphaned do this:
```shell
sudo deborphan -sz
```

Then remove all orphaned files:
```shell
sudo apt-get remove --purge $(deborphan)
```

Finally do this to remove the unnecessary packages that are not longer needed:
```shell
sudo apt-get autoremove
```

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
To clone our repository, install GIT:
```shell
sudo apt-get install git
```

Clone the repository:
```shell
cd ~
git clone https://github.com/janhieber/WaterCtrl.git
```


## SSH access
*Note: You don't need SSH, you can still use serial connection or a terminal over HDMI,
but SSH makes it easier*

TODO: Is SSH installed by default? Is it running by default?

Set the allowed SSH users. This is very important, so that not everybody in your network
is able to connect to your Pi!
```shell
echo -e "\nAllowUsers pi\n" | sudo tee -a /etc/ssh/sshd_config
sudo service ssh restart
```
With this, only the *pi* user has SSH access and we don't need to worry about password of
the other system users.



## Install database

We will use PostgreSQL.
```shell
sudo apt-get install postgresql postgresql-contrib postgresql-client php5-pgsql python3-postgresql
```

### Configure PostgreSQL:
When creating the user, a password will be assigned.  
We name this user *pi*, because when you are logged in as shell user *pi*,
PostgreSQL translates the username and grants you permissions.

```shell
sudo su - postgres
createuser -P pi
createdb -O pi waterctrl
exit

```

### Access from outside
If you want to manage our DB from outside
with a GUI like [pgadmin](http://www.pgadmin.org), you can make
the database accessible over network.  
Be careful, that means that everybody in your home network can connect
to the database!

Edit `/etc/postgresql/9.4/main/pg_hba.conf`:  
Add `host all all 192.168.0.0/24 md5` to allow a whole network.  
Add `host all all 192.168.0.123/32 md5` to allow a specific IP.

Further information: [http://www.postgresql.org/docs/9.1/static/auth-pg-hba-conf.html](http://www.postgresql.org/docs/9.1/static/auth-pg-hba-conf.html)

Then change  
`listen_addresses = 'localhost'`  
to  
`listen_addresses = '*'`  
in `/etc/postgresql/9.4/main/postgresql.conf`

If you forgot to set a password for the SQL user before:
```shell
echo "ALTER ROLE pi WITH PASSWORD 'secure123'" | psql
```

Restart PostgreSQL
```shell
sudo service postgresql restart
```

### Create a database

#### MYSQL (deprecated)

	mysql -u root -p
	CREATE DATABASE waterctrl;
	CREATE USER 'waterctrl_user'@'localhost' IDENTIFIED BY 'Wfmd22nGixsP';
	GRANT ALL ON waterctrl.* TO 'waterctrl_user'@'localhost';
	FLUSH PRIVILEGES;

##### PSQL

```
alarm# sudo su postgres
postgres$ ALTER ROLE pi WITH LOGIN;
postgres$ CREATE ROLE waterctrl_user WITH PASSWORD 'Wfmd22nGixsP';
postgres$ ALTER ROLE waterctrl_user WITH LOGIN;
postgres$ GRANT ALL PRIVILEGES ON DATABASE waterctrl to waterctrl_user;
```

## Install dependencies for webinterface

We will need PIP for our webinterface dependencies:

	sudo apt-get install python-pip python-dev

Then go to the webinterace folder

	cd /home/pi/WaterCtrl/WebIf/Webserver

Install the dependencies:

	sudo pip install -r requirements.txt


## Run the webserver as a daemon

Save the following script in a file called `/etc/init.d/webinterface`
```shell
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
```

Make the script executable `chmod +x /etc/init.d/webinterface`.  
Finally, update the daemons `sudo update-rc.d webinterface defaults`.

Now you can start the webserver via the command `service webinterface start`.

Same must be done accordingly for the background control daemon. Please use file `waterctrl` .

Install service via `insserv` instead of `update-rc.d` to ensure service start after webinterface and postgresql.
```
sudo insserv -d waterctrl
```


## Create tables for database
TODO: rewrite to PostgreSQL

```shell
python /home/pi/WaterCtrl/WebIf/Webserver/manage.py create_db
```

## Add sample data
You only need this for testing.  
TODO: Do we need testdata?

```shell
cd /home/pi/WaterCtrl/WebIf/Webserver/
python manage.py seed_db --seedfile 'data/db_user.json'
python manage.py seed_db --seedfile 'data/db_log.json'
python manage.py seed_db --seedfile 'data/db_motor.json'
python manage.py seed_db --seedfile 'data/db_sensor.json'
python manage.py seed_db --seedfile 'data/db_plant.json'
python manage.py seed_db --seedfile 'data/db_watering.json'
```

## Open browser

Finally your webinterface should be up and running on port 80. Open your browser and test it!
