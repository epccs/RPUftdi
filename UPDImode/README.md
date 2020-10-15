# UPDI mode

## Overview

The BCM23 and BCM24 pins are used by my PiUPDI board to select serial0 and UPDI programing. PiUPDI has an AVR128DA28 that is programed with [pyupdi].

[pyupdi]: https://github.com/mraardvark/pyupdi/


## Firmware Upload

Use the secondary serial port connection (see BOOTLOAD_PORT in Makefile) to reset the MCU and execute optiboot so that the 'make bootload' rule can upload a new binary image in the application area of flash memory. The bootloader runs at 38.4kbps.

``` 
sudo apt-get install make git picocom gcc-avr binutils-avr gdb-avr avr-libc avrdude
git clone https://github.com/epccs/RPUusb/
cd /Gravimetric/Applications/BCM24cntl
make all
...
avr-objcopy -j .text -j .data -O ihex blinkLED.elf blinkLED.hex
avr-objdump -h -S blinkLED.elf > blinkLED.lst
make bootload
...
avrdude done.  Thank you.
``` 

Now connect with picocom (or ilk) to send commands.

``` 
picocom -b 38400 /dev/ttyUSB1
...
/0/id?
{"id":{"name":"BMC24cntl","desc":"RPUusb (14145^5) Board /w ATmega328pb","avr-gcc":"5.4.0"}}
/0/updi
{"PiUPDI":"UPDI"}
/0/uart
{"PiUPDI":"UART"}
#exit is C-a, C-x
``` 

The Python program will need to be done so it can reconize if a R-Pi is in use or if it is an RPUusb.
