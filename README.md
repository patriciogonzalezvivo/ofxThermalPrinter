[![Adafruit Mini Thermal Printer](http://www.adafruit.com/images/970x728/597-00.jpg)](http://www.adafruit.com/products/597)

This [openFrameworks](http://www.openframeworks.cc/) addon let you connect Adafruit's Mini Thermal Printer to any of your [openFrameworks](http://www.openframeworks.cc/) projects.

# Installing 

Clone [this repository](https://github.com/patriciogonzalezvivo/ofxThermalPrinter) into your `openFrameworks/addons` folder:

	cd openFrameworks/addons
	git clone https://github.com/patriciogonzalezvivo/ofxThermalPrinter

# Setting up

## On MacOS/Linux

You can use the printer directly from your computer using an [USB to TTL Serial Cable](http://www.adafruit.com/products/954)

You need to research witch port open in the `/dev/`. Should look like this:

	printer.open("/dev/tty.PL2303-00002014");

## on RaspberryPi

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

The in your code open the connection at:

	printer.open("/dev/ttyAMA0");

# Usage 

Once the printer connection is open you can print text:

	printer.println("hello world");

Dithered images:

	printer.printImage(ofImage("image.jpg"));

And Barcodes:

	printer.printBarcode("12345678", EAN8);

You will also find that there are lot's of ways to style your text;

	printer.setAlign(LEFT);
	printer.setAlign(MIDDLE);
	printer.setAlign(RIGHT);

	printer.setBold(true);
  printer.setBold(false);

	printer.setReverse(true);
	printer.setReverse(false);

  printer.setUnderline(true);
	printer.setUnderline(false);

Happy projects for you ;)

<iframe class="vine-embed" src="https://vine.co/v/MejwEIm6ael/embed/simple" width="575" height="575" frameborder="0"></iframe><script async src="//platform.vine.co/static/scripts/embed.js" charset="utf-8"></script>