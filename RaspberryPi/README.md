# Raspberry Pi
This is for the Raspberry Pi only.

If you want to setup everything manually, follow [ManualSetup.md](./ManualSetup.md).

Otherwise write the latest image to the sdcard and configure as follows.

## Write image to SD
### Linux / OSX
Open a terminal and type
```bash
sudo su
```
Now you should have root privileges.
Go to your downloads folder and write the image with dd to your SD card:
```bash
cd downloads
dd if=WaterCtrl_rpi.img of=/dev/mmcblk0 bs=4M
sync
```

### Windows
Use Win32DiskImager to write the image file on the SD card.

