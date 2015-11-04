This is a Howto to reproduce our minimal system image.

We use this to create new images when new Raspbian versions are released.

# Prereq
- Minimum SDCard size: 8 GB

# Basics
_Clean the SDCard with the following command:
```bash
dd if=/dev/zero of=/dev/mmcblk0 bs=4M
sync
```

Download. extract and write the image:
```bash
unzip 2015-09-24-raspbian-jessie.zip
dd if=2015-09-24-raspbian-jessie.img of=/dev/mmcblk0 bs=4M
sync
rm 2015-09-24-raspbian-jessie.{zip,img}
```

# Boot config
Enable SPI:
```bash
mount /dev/mmcblk0p1 /mnt
nano /mnt/config.txt
```
Remove comment on line:
>#dtparam=spi=on

Unmount, sync
```bash
umount /mnt
sync
```

Now boot the RPi, attached UART cable before boot.

# Network config
Move and link network configs:
```bash
mv /etc/network/interfaces /boot/
ln -s /boot/interfaces /etc/network/interfaces
mv /etc/wpa_supplicant/wpa_supplicant.conf /boot/
ln -s /boot/wpa_supplicant.conf /etc/wpa_supplicant/wpa_supplicant.conf
```

Edit /boot/wpa_supplicant.conf and insert the WLAN config:
>network={
>  ssid="jou"
>  psk="EXei3fie"
>  key_mgmt=WPA-PSK
>}

Reboot and check network connection.

# Upgrade system
```bash
sudo apt-get update
sudo apt-get dist-upgrade
sync
rm -rf python_games

sudo apt-get purge xserver* x11-common x11-utils x11-xkb-utils x11-xserver-utils xarchiver xauth xkb-data console-setup xinit lightdm libx{composite,cb,cursor,damage,dmcp,ext,font,ft,i,inerama,kbfile,klavier,mu,pm,randr,render,res,t,xf86}* lxde* lx{input,menu-data,panel,polkit,randr,session,session-edit,shortcut,task,terminal} obconf openbox gtk* libgtk* alsa* nano scratch tsconf desktop-file-utils omxplayer

sudo apt-get autoremove
sudo apt-get install python3-rpi.gpio python-rpi.gpio wiringpi console-data vim
reboot
sudo dpkg-reconfigure tzdata
sudo dpkg-reconfigure console-data
sudo apt-get install mariadb-server mariadb-client python3-mysql.connector 
```

to be continued ...
