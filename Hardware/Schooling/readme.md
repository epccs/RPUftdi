# Description

Some lessons I learned doing RPUusb.

# Table Of Contents:

1. ^5 Smoke
1. ^5 Names
1. ^4 Power Chopping
1. ^3 Baud Rate
1. ^3 Bus Manager ICP1 Used to Hault Host
1. ^3 FT231X Problem Toggling nDTR and nRTS
1. ^2 I2C Level Converter
1. ^2 Brick Bus Management MCU


## ^5 Smoke

Do not power the RPUusb board (e.g., so remove the USB plug before) first. Power the application (Gravimetric) board and then plug in the USB. This problem is going to take some time to understand. First, I have to figure out which part(s) let out smoke; it is not apparent.

Found a damaged place, it is the manager (m328pb on Gravimetric^2) over the power pin four, which has a bump above it where the smoke was let out. I have found a few paths (TWI1 which is used for SMBus) where the current from RPUusb could flow into an unpowered manager. I suspect that when I had the USB powered and the Gravimetric unpowered, it damaged the manager, and when I powered the Gravimetric, the manager let out its smoke since it was degraded. I think this could happen without severely damaging other parts, so I will try to replace the Gravimetric manager.

This is probably when a second power domain on an MCU makes  sense. The new AVR-DB series offers it but that will need to be an update, also I think it will need to be the manager on Gravimetric that gets the update.

Replaceing the manager on Gravimetric did fix the problem so if a AVR-DB has two I2C ports on sepreate power domains than I may be fix this. An externaly powered R-Pi would have the same problem I suspect, so this needs fixed (but not on the RPUusb board, it needs fixed on the application board.)


## ^5 Names

RPUftdi has been renamed RPUusb because I am not using an FTDI chip for this. It was probably a bad idea to use the company name in the board name like that.


## ^4 Power Chopping

Transceivers start and stop (chop) the power flow over the USB cable; occasionally it results in a reset of the host computer. Mixing power and data is risky; the next version of this board should not power the transceivers from the USB.


## ^3 Baud Rate 

The [Bit Rate] error at 115.2k is too high (though it mostly works). I have not seen errors on Windows yet but am seeing them on Linux. Looking at the [Rate Calculator] I see 76.8k is very good, but the picocom packaged on Ubuntu does not support it. The next very good rate is 38.4k which picocom does support. The bootloader does a CRC check with each line in the hex file so running it 115.2k is fine, it will redo the chunks with errors. However, I don't want to see any errors while running commands (that would look bad during a demo).

Note: [Baud] is the wrong term, but is the one everyone knows.

[Rate Calculator]: http://wormfood.net/avrbaudcalc.php
[Baud]: https://en.wikipedia.org/wiki/Baud
[Bit Rate]: https://en.wikipedia.org/wiki/Bit_rate#Gross_bit_rate


## ^3 Bus Manager ICP1 Used to Hault Host

Connect bus manager ICP1 pin to a test point rather than DTR transceiver. RPUpi is using it to shutdown/halt the host computer. Cut the indicated PCB trace to open the ICP1 pin.

![Open ICP1](./14145^3_OpenICP1.png "Open ICP1")


## ^3 FT231X Problem Toggling nDTR and nRTS

This problem is not hardware, it was caused because I wrote the Host2Local firmware as though nSS was connected to nRESET directly but that is not the case. I have an open drain buffer between them so when  nSS went high impedance it did not pull up the buffer and the MCU did not turn on after reset. I am still confused why nDTR and nRTS did not pull up but they seem to be working as expected (on both Linux and Windows) now that I set nSS as an output with a high and low state (as opposed to a low and high-z state).


## ^2 I2C Level Converter

If I2C has a pull-up to 3.3V then the level converter is not needed. Arduino's IDE Wire C++ class turns on the ATmega328 internal pull-ups which raise the voltage above 3.3, e.g. I read 3.5V for an undriven bus, which is probably fine but it confused and was not expected. After un-wrapping the TWI code from the Wire class and getting it to run with the vanilla avr-libc I found the pull-up code and turned it into an option during initialization.


## ^2 Brick Bus Management MCU

R20 (jumpered by default) was used to connect a local MCU (SS from a Uno) to reset the bus manager. Unfortunaly when the bootloader programs the local boad it seems to play with the SPI interface and ends up setting the flages of the bus manager to zero. This effectivly bricks the part, though the flags can be set with a parall programing setup (which I may do after collecting a few bricks).

SPI to the bus manager has been removed. 
