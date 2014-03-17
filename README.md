# RaspberryPi setup

Follow [this](http://learn.adafruit.com/pi-thermal-printer/soldering) instructions for the connection of the wires and GPIOs. (Also [here](http://natemcbean.com/2012/11/rpi-thermal-printer/) is another nice tutorial)

By default the TXD and RXD pins are connected to a console. We need rid of this by editing this file:

	sudo nano /boot/cmdline.txt

Delete all references to 'ttyAMA0'. So you will have something like this:

	dwc_otg.lpm_enable=0 console=ttyAMA0,115200 kgdboc=ttyAMA0,115200 console=tty1 root=/dev/mmcblk0p2 rootfstype=ext4 rootwait

and will look like this:

	dwc_otg.lpm_enable=0 console=tty1 root=/dev/mmcblk0p2 rootfstype=ext4 rootwait

Then edit the /etc/inittab

	sudo nano /etc/inittab 

and find the following line:

	T0:23:respawn:/sbin/getty -L ttyAMA0 115200 vt100

Restart your RaspberryPi using:

	sudo reboot
