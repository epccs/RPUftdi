# Host2Remote

This is a work in progress... 

This bus manager firmware is for an RPUftdi board, it will send a byte (address) over DTR pair to reset a MCU board (e.g. an Irrigate7 with an RPUadpt shield) when the FTDI_nDTR toggles. The application firmware once loaded and running on MCU board can then enable normal mode when it starts or wait for the lockout to timeout. 

## Overview

In normal mode the RS-422 pairs have RX/TX driver and receiver enabled. The RS-485 (DTR) pair has UART data sent from the bus manager that depends on settings from I2C/TWI (but defaults to sending a '0') when FTDI_nDTR toggles.

The USB UART outputs FTDI_TX that drives the RX pair, which can be seen by all nodes on the RPU_BUS (if not in lockout mode). While the FTDI_RX takes input from the TX pair receiver which is hopfully driven by only one of the RPU_BUS nodes (the requirement is to not talk unless commanded to talk, and stop everything if a new command is started). 

FTDI_nDTR signal will cause a byte to be sent on the DTR pair to reset a MCU board on the bus and lockout the others. DTR_DE is held high to enable the DTR pair driver. DTR_nRE is held low to enable the DTR pair receiver. 

## Addressing

This firmware is at Address '0' on the RPU_BUS, (not zero but the ASCII value for the character).

When DTR toggles an address byte will be sent out over the DTR pair. If the address sent is '0' the local MCU will remain connected to the RPU_BUS, for all other address the local MCU will be locked out. After a period of time in lockout or when a normal mode byte (0xFF) is seen on DTR pair, the MCU is (re)connected to the RPU_BUS. 

## I2C/TWI Slave

The I2C address is 0x29 (dec 41). It is organized as an array of read or write commands. Note: the sent data is used to size the reply, so add an extra byte after the command to size the reply.

0: reads this shields RPU_BUS addrss (default is '0')
1: writes this shields RPU_BUS address (not implemented)
2: read the address sent when DTR/RTS toggles 
3: write the address that will be sent when DTR/RTS toggles
4: reads TBD (not implemented)
5: wrtie normal RPU_BUS mode (send 0xFF on DTR pair) that will cancel lockout and connect all devices to the bus.
6: reads error status bits[0:DTR readback timeout, 1:twi transmit fail, 2:DTR readback not match]
7: wrties (or clears) error status 


Connect to i2c-debug on an RPUno with picocom (or ilk). 

``` 
#exit picocom with C-a, C-x
picocom -b 115200 /dev/ttyUSB0
``` 

Scan for the I2C slave address of the RPUftdi shield.

``` 
/0/id?
{"id":{"name":"I2Cdebug","desc":"RPUno Board /w atmega328p and LT3652","avr-gcc":"4.9"}}
/0/scan?
{"scan":[{"addr":"0x29"}]}
/0/address 41
{"address":"0x29"}
``` 

The local RPU address can be read.

``` 
/0/buffer 0,255
{"txBuffer":[{"data":"0x0"},{"data":"0xFF"}]}
/0/read? 2
{"rxBuffer":[{"data":"0x0"},{"data":"0x30"}]}
``` 

Set the byte that is sent when DTR/RTS toggles ('1' is 0x31 or 49).

``` 
/0/buffer 3,49
{"txBuffer":[{"data":"0x3"},{"data":"0x31"}]}
/0/read? 2
{"rxBuffer":[{"data":"0x3"},{"data":"0x31"}]}
``` 

Note: closeing picocom and starting it agian will show a slow blink on the RPUftid LED durring lockout and then a normal blink until picocom is closed.


Set normal mode to connect all devices to the RPU_BUS.

``` 
/0/buffer 5,255
{"txBuffer":[{"data":"0x5"},{"data":"0xFF"}]}
/0/read? 2
{"rxBuffer":[{"data":"0x5"},{"data":"0xFF"}]}
``` 
