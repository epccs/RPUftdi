# Description

This shows the setup and method used for evaluation of RPUftdi.

# Table of References


# Table Of Contents:

3. [^3 Proof of Concept](#3-proof-of-concept)
2. [^2 I2C Slave Operation](#2-i2c-slave-operation)
1. [^2 RS485 Full Duplex Loopback](#2-rs485-full-duplex-loopback)


## ^3 Proof of Concept

The proof of concept (e.g. Multi-Drop Remote Bootload) used [Host2Remote] firmware on a RPUftdi^3 and [i2c-debug] on its bare metal MCU board (an OSEPP Uno R3 from fry's electronics). The remote device has [Remote] firmware on a RPUadpt^3 board mounted on an Irrigate7^1 (which is an ATmega1284p with xboot). The firmware bootloaded onto the Irrigate7^1 is [BlinkLED].

[Remote Bootload Video](http://rpubus.org/Video/14145%5E3_RPU_RemoteBootload.mp4 "Remote Bootload Video")

![Remote Bootload](./14145^3_RemoteBootload.jpg "Remote Bootload")

[Host2Remote]: https://github.com/epccs/RPUftdi/tree/master/Host2Remote
[Remote]: https://github.com/epccs/RPUadpt/tree/master/Remote
[BlinkLED]: https://github.com/epccs/Irrigate7/tree/master/BlinkLED


## ^2 I2C Slave Operation

I2C Slave operation has been checked with the [TwiSlave] firmware on a RPUftdi^2 with [i2c-debug] on an OSEPP Uno R3 (from fry's electronics).

![I2C Check Setup](./14145^2_I2cCheckSetup.jpg "I2C Check Setup")

[TwiSlave]: https://github.com/epccs/RPUftdi/tree/master/TwiSlave
[i2c-debug]: https://github.com/epccs/RPUno/tree/master/i2c-debug


## ^2 RS485 Full Duplex Loopback

Checking voltage levels and sending data through a loop back shows the RS485 TX and RX Transceivers do what is needed. Firmware Host2Local enables both TX and RX transceiver driver and receiver to check this.

![485 Loop Back](./14145^2_485LoopBack.jpg "485 Loop Back")

