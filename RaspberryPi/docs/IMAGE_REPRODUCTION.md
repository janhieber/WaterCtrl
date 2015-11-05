This is a Howto to reproduce our minimal system image.

We use this to create new images when new Raspbian versions are released.

# Prereq
- Minimum SDCard size: 8 GB

# Notes
- We can post our MariaDB/MySQL password here, because network access is disabled by default

# Install Rasbian
### Clean the SDCard
```shell
dd if=/dev/zero of=/dev/mmcblk0 bs=4M
sync
```

### Download. extract and write the image
```shell
unzip 2015-09-24-raspbian-jessie.zip
dd if=2015-09-24-raspbian-jessie.img of=/dev/mmcblk0 bs=4M
sync
rm 2015-09-24-raspbian-jessie.{zip,img}
```

### Boot config
Enable SPI:
```shell
mount /dev/mmcblk0p1 /mnt
nano /mnt/config.txt
```
Remove comment on line:
>\#dtparam=spi=on

### Unmount, sync
```shell
umount /mnt
sync
```

Now boot the RPi, attached UART cable before boot.

# Network config
### Move and link network configs
```shell
mv /etc/network/interfaces /boot/
ln -s /boot/interfaces /etc/network/interfaces
mv /etc/wpa_supplicant/wpa_supplicant.conf /boot/
ln -s /boot/wpa_supplicant.conf /etc/wpa_supplicant/wpa_supplicant.conf
```

### Configure network
Edit /boot/wpa_supplicant.conf and insert the WLAN config:
>network={  
>  ssid="ssid"  
>  psk="psd"  
>  key_mgmt=WPA-PSK  
>}  

Reboot and check networking.

### Protect config links
When updating packages, the symlinks may get overwritten.
We check this in a script and recreate them on shutdown:
```shell
sudo wget https://raw.githubusercontent.com/janhieber/WaterCtrl/master/RaspberryPi/scripts/rc.local.shutdown -O /etc/rc.local.shutdown
sudo chmod +x /etc/rc.local.shutdown
```


# Upgrade system
```shell
sudo apt-get update
sudo apt-get dist-upgrade
sudo rpi-update
sync
rm -rf python_games

sudo apt-get purge xserver* x11-common x11-utils x11-xkb-utils x11-xserver-utils xarchiver xauth xkb-data console-setup xinit lightdm libx{composite,cb,cursor,damage,dmcp,ext,font,ft,i,inerama,kbfile,klavier,mu,pm,randr,render,res,t,xf86}* lxde* lx{input,menu-data,panel,polkit,randr,session,session-edit,shortcut,task,terminal} obconf openbox gtk* libgtk* alsa* nano scratch tsconf desktop-file-utils omxplayer

sudo apt-get autoremove
sudo apt-get install python3-rpi.gpio python-rpi.gpio wiringpi console-data vim nano ca-certificates

reboot
sudo dpkg-reconfigure tzdata
sudo dpkg-reconfigure console-data
```

# WaterCtrl specific
### Clone repo
```shell
git clone https://github.com/janhieber/WaterCtrl.git
```

### Install spidev for python
```shell
sudo apt-get install python3-spidev 
```
### Install MariaDB
```shell
sudo apt-get install mariadb-server mariadb-client python3-mysql.connector
```

### Create WaterCtrl database
```shell
cd ~/WaterCtrl/RaspberryPi/scripts
mysql -u root -p < create_db.sql
```




# Create image
### Remove sensible data!
- WLAN password

Others:
```shell
for i in /home/pi/.ssh /home/pi/.gitconfig /root/.gitconfig /root/.ssh /home/pi/.bash_history /root/.bash_history; do sudo rm -Rf "$i"; done
```

### Wipe free space
```shell
sudo apt-get install secure-delete
sudo sfill -llz /
sync
sudo poweroff
```

### Remove SDCard and create image + zip
```shell
dd if=/dev/mmcblk0 conv=sync,noerror bs=4M | gzip -c  > image.img.gz
```

### To restore the image:
```shell
gunzip -c image.img.gz | dd of=/dev/mmcblk0 bs=4M
sync
```

#to be continued ...
