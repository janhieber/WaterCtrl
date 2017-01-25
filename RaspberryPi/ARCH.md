## Get ARCH arm SD card image

Download the rpi latest image from the Official homepage.

[archlinux|ARM](http://archlinuxarm.org)

Good hints for wifi setup found here.
[archpi](http://archpi.dabase.com/)

## System setup

Before first boot of sd card, add these line to __/boot/config.txt__

```
dtparam=i2c_arm=on
device_tree_param=spi=on
loglevel=5
```

### Enable wifi network
Setup wifi credentials via wpa_supplicant.

__/etc/wpa_supplicant/wpa_supplicant-wlan0.conf__

```
ctrl_interface=/run/wpa_supplicant
update_config=1

network={
# scan_ssid=1 # uncomment for hidden ssid
        ssid="YourSSID"
        psk="YourPassword"
}
```

Enable dhcp with systemd-networkd

_/etc/systemd/network/wlan0.network_

```
[Match]
Name=wlan0

[Network]
DHCP=yes
```

## First system run

First you have to install the missing sudo package with pacman. This can only be done as root. Therefore connect the serial port to your pc and log in via console. The _root_ password _root_.

```
[root@alarmpi ~]# pacman -S sudo
[root@alarmpi ~]# visudo
```
``alarm ALL=(ALL) ALL``

Now yo can connect vi ssh and use the regular user to install the rest of this guide.

Default host name is 'alarmpi', user _alarm_ with password _alarm_.

__DON'T FORGET TO CHANGE THE PASSWORDS__

### set locale for python
Generate and activate desired language.

[arch locale setup](https://wiki.archlinux.org/index.php/locale)

Edit __/etc/locale.conf__ and add these lines, e.g. _en_US.UTF-8_.

```
LC_CTYPE=en_US.UTF-8
LC_ALL=en_US.UTF-8

```

### Install needed packages

```
pacman -S postgresql postgresql-libs python python2 python-six python-pip python2-pip git gcc

pip install -r WaterCtrl/WebIf/Webserver/requirements_arch.txt

pip2.7 install -r WaterCtrl/WebIf/Webserver/requirements.txt
```

wsgiref add as library to python 3.5. and six on arch installable aur package and does not need to be installed by pip for python3. For python2.7 we still have to install six using pip2.7.

## Project setup

[ManualSetup.md](./ManualSetup.md).

### postgres

Create database user _alarm_ instead of _pi_, because we are logged in as alarm.

Init database directory in filesystem.

`alarm# sudo su - postgres`

`postgres$  initdb --locale $LANG -E UTF8 -D '/var/lib/postgres/data'`

Enable and start service.

`[alarm@alarmpi scripts]$ sudo systemctl enable postgresql`

`[alarm@alarmpi scripts]$ sudo systemctl start postgresql`


Now its possible to set up user and database.

```
postgres$ createuser -P alarm
postgres$ createdb -O alarm waterctrl
postgres$ psql waterctrl

```

To setup tables follow [WebIf Setup](../WebIf/Webserver/README.md)

### Source

Clone our github repo to get the source.
`git clone https://github.com/janhieber/WaterCtrl`

### Autostart

ARCH comes with systemd. We have to write 2 service files and copy them to the systems systemd directory in order get daemon and webserver started during system boot. The _webinterface.service_ starts after network.target, followed by _waterctrl.service_.

```
alarm# cd ~/WaterCtrl/raspberrypi/scripts
alarm# sudo cp *.service /etc/systemd/system
alarm# systemctl enable webinterface.service
alarm# systemctl start webinterface.service
alarm# systemctl enable waterctrl.service
alarm# systemctl start waterctrl.service
