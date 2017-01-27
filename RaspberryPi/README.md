# Raspberry Pi
This is for the Raspberry Pi only.

If you want to setup everything manually, follow [ManualSetup.md](./ManualSetup.md).

Otherwise write the latest image to the sdcard and configure as follows.

## Write image to SD
### Linux / OSX
Download the current image file from [http://share.janhieber.net/WaterCtrl/](http://share.janhieber.net/WaterCtrl/)

[ARCH image file](https://c.janhieber.net/index.php/s/xm1jYHjHfZumiCj?path=%2Falarmpi)
If the image is splitted, recreate the original file.

`cat file.gz.part* > file.gz`

In a terminal go to your downloads folder and write the image to SDCard:
```bash
cd downloads
sudo su
gunzip -c DATE-sdcard.img.gz | dd of=/dev/mmcblk0
sync
```
