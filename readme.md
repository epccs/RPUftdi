# RPUftdi

From <https://github.com/epccs/RPUftdi>

## Overview

Shield used to connect a microcontroller and a USB host to a full duplex RS-422 (RX and TX pairs) and an out of band half duplex RS-485 (DTR pair) over CAT5. It's a multidrop bus between a host (e.g. Pi Zero on [RPUpi] or no host with [RPUadpt]) and an MCU board (e.g. [RPUno]).

[HackADay](https://hackaday.io/project/15585-rpuftid)

[Forum](http://rpubus.org/bb/viewforum.php?f=5)

[OSHpark](https://oshpark.com/shared_projects/7gJOSPpM)

[RPUno]: https://github.com/epccs/RPUno
[RPUpi]: https://github.com/epccs/RPUpi
[RPUadpt]: https://github.com/epccs/RPUadpt

## Status

![Status](./Hardware/status_icon.png "Status")


## [Hardware](./Hardware)

Hardware files are in Eagle, there is also some testing, evaluation, and schooling notes for referance.


## Example

A multi-drop serial bus allows multiple microcontroller boards to be connected to a host serial port. The host computer needs to be near the RPUftdi board because USB works over short distances (about 2 meters). The host serial handshake signals are connected to the bus manager microcontroller which is also connected to the transceivers. The goal is to have a multidrop connection that configures automatically when the host activates the FTDI UART bridge.  Additionally, it is desired to have zero software [magic] for control of the transceivers. The microcontrollers also do not need to control the transceivers, but only one should be allowed to talk.

[magic]: https://github.com/pyserial/pyserial/blob/master/serial/rs485.py

![MultiDrop](./Hardware/Documents/MultiDrop.png "MultiDrop")

In the examples, I have for [RPUno] a command processor accepts interactive textual commands and operates the peripherals. The examples have a simple makefile that compiles the microcontroller firmware from the source. The host I use has Ubuntu (or Raspbian) with the AVR toolchain installed.

The makefile has a rule (e.g. "make bootload") that uploads to the targets bootloader with avrdude. Since the transceiver control [magic] is pushed into the bus manager, avrdude can work (the bus manager initializes a point to point mode with a 25 second time to live when the host becomes active). 

When PySerial on the host opens the serial port it pulls the nDTR line low (it is active low) and that tells the manager running [Host2Remote] firmware to reset the bootload address (broadcast on DTR pair). PySerial needs to wait for a few seconds so the bootloader timeout finishes. The RPUno application needs to read the RPUftid serial address over I2C so that the RPUftdi bus manager knows to connect the RPUno's serial lines to the multi-drop bus (and end the bootload mode immediately).

[Host2Remote]: ./Host2Remote

When avrdude opens the serial port it pulls the nDTR line low and the manager broadcast the bootload address which places everything in lockout except the host and the microcontroller board that was addressed. The address is held in the manager on the shield so replacing the [RPUno] dos not change that locations address, but replacing the [RPUadpt] does.

## AVR toolchain

This is easy because it is packaged. Thanks Debain, Ubuntu, Raspibin, ...

* sudo apt-get install [gcc-avr]
* sudo apt-get install [binutils-avr]
* sudo apt-get install [gdb-avr]
* sudo apt-get install [avr-libc]
* sudo apt-get install [avrdude]
    
[gcc-avr]: http://packages.ubuntu.com/search?keywords=gcc-avr
[binutils-avr]: http://packages.ubuntu.com/search?keywords=binutils-avr
[gdb-avr]: http://packages.ubuntu.com/search?keywords=gdb-avr
[avr-libc]: http://packages.ubuntu.com/search?keywords=avr-libc
[avrdude]: http://packages.ubuntu.com/search?keywords=avrdude