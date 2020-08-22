# Makefile based non-blocking Blink

## Overview

Demonstration of General I/O, e.g. Blink an LED. 

It also links an Interrupt-Driven UART library for AVR Standard IO facilities, as well as some Python that sends an 'a' character to stop blinking the SCK pin. 

## Firmware Upload

Use the secondary serial port connection (see BOOTLOAD_PORT in Makefile) to reset the MCU and execute optiboot so that the 'make bootload' rule can upload a new binary image in the application area of flash memory. The bootloader runs at 38.4kbps.

``` 
sudo apt-get install make git picocom gcc-avr binutils-avr gdb-avr avr-libc avrdude
git clone https://github.com/epccs/Gravimetric/
cd /Gravimetric/Applications/BlinkLED
make all
...
avr-objcopy -j .text -j .data -O ihex blinkLED.elf blinkLED.hex
avr-objdump -h -S blinkLED.elf > blinkLED.lst
make bootload
...
avrdude done.  Thank you.
``` 

Next use picocom to send characters, an 'a' to stop the blinking of the SCK pin (there is no LED), characters also echo.

``` 
picocom -b 38400 /dev/ttyUSB1
...
a
#exit is C-a, C-x
``` 

