# RPUftdi

From <https://github.com/epccs/RPUftdi>

## Overview

USB interface for the RPUbus. It plugs into an R-Pi header (e.g., [RPUpi] or [Gravimetric]). The full-duplex multidrop bus is directly connected to one of the duel USB UART's, while the second USB UART is connected to an MCU (ATmega328pb) that can act as a bridge for many of the R-Pi header functions (e.g., SPI, I2C) from the target board. A USB host will be able to set the bootload address on the RPUbus with the I2C interface bridged, initiate a shutdown, and other functions that an R-Pi has access to from the manager.

[Gravimetric]: https://github.com/epccs/Gravimetric
[RPUpi]: https://github.com/epccs/RPUpi


## Status

[Available](https://rpubus.org/Order_Form.html)

![Status](./Hardware/status_icon.png "Status")

This is programmed with an in-circuit serial programming tool that is able to handle the 3.3V ATmega328pb target. I use an Arduino Uno with the [ArduinoISP] sketch and an SPI level shifter. A Raspberry Pi also works with the [ICSP].

[ICSP]: https://github.com/epccs/Driver/tree/master/ICSP

[ArduinoISP]: https://github.com/arduino/Arduino/blob/master/build/shared/examples/11.ArduinoISP/ArduinoISP/ArduinoISP.ino

## [Hardware](./Hardware)

Hardware files and notes for referance.


## Example

TBD

## AVR toolchain

The core files for this board are in the /lib folder. Each example has its files and a Makefile in its own folder. The toolchain packages that I use are available on Ubuntu, Raspbian and Windows with WSL and Ubuntu installed. 

```
sudo apt-get install git make gcc-avr binutils-avr gdb-avr avr-libc avrdude
git clone https://github.com/epccs/RPUftdi
```

* [gcc-avr](https://packages.ubuntu.com/search?keywords=gcc-avr)
* [binutils-avr](https://packages.ubuntu.com/search?keywords=binutils-avr)
* [gdb-avr](https://packages.ubuntu.com/search?keywords=gdb-avr)
* [avr-libc](https://packages.ubuntu.com/search?keywords=avr-libc)
* [avrdude](https://packages.ubuntu.com/search?keywords=avrdude)
