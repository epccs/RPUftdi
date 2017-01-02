# Host2Remote

This bus manager firmware is for an RPUftdi board, it will send a byte (address) over DTR pair to reset a MCU board (e.g. an Irrigate7 with an RPUadpt shield) when the FTDI_nDTR toggles. The application firmware once loaded and running on MCU board can then enable normal mode when it starts or wait for the lockout to timeout. 

## Overview

In normal mode the RS-422 pairs have RX/TX driver and receiver enabled. The RS-485 (DTR) pair has UART data sent from the bus manager that depends on settings from I2C/TWI (but defaults to sending a '0') when FTDI_nDTR toggles.

The USB UART outputs FTDI_TX that drives the RX pair, which can be seen by all nodes on the RPU_BUS (if not in lockout mode). While the FTDI_RX takes input from the TX pair receiver which is hopfully driven by only one of the RPU_BUS nodes (the requirement is to not talk unless commanded to talk, and stop everything if a new command is started). 

FTDI_nDTR signal will cause a byte to be sent on the DTR pair to reset a MCU board on the bus and lockout the others. DTR_DE is held high to enable the DTR pair driver. DTR_nRE is held low to enable the DTR pair receiver. 

## Firmware Upload

With an ISP tool connected to the bus manager (set the ISP_PORT in Makefile) run 'make isp' and it should compile and then flash the bus manager.

``` 
rsutherland@conversion:~/Samba/RPUftdi/Host2Remote$ make isp
...
avrdude done.  Thank you.
``` 

## Addressing

The Address '0' on the RPU_BUS is 0x30, (e.g. not 0x1 but the ASCII value for the character).

When DTR (or RTS) toggles from a host connected to the RPU bus the receiving bus manager will set localhost_active and send the bootloader_address over the DTR pair. If an address received by way of the DTR pair matches the local RPU_ADDRESS the bus manager will enter bootloader mode (marked with bootloader_started), and connect the node MCU to the RPU_BUS (see connect_bootload_mode() function), all other address should be locked out. After a LOCKOUT_DELAY time or when a normal mode byte is seen on the DTR pair, the lockout ends and normal mode resumes. The node that has bootloader_started broadcast the return to normal mode byte on the DTR pair when the node reads the RPU_ADDRESS from the bus manager over I2C (otherwise it will time out).


## Bus Manager Modes

In Normal Mode, the RPU bus manager connects the local MCU node to the RPU bus if it is RPU aware (e.g. ask for RPU address over I2C). Otherwise, it will not connect the local MCU's TX to the bus but does connect RX. The host will be connected unless it is foreign.

In bootload mode, the RPU bus manager connects the local MCU node to the RPU bus. Also, the host will be connected unless it is foreign. It is expected that all other nodes are in lockout mode. Note the BOOTLOADER_ACTIVE delay is less than the LOCKOUT_DELAY, but it needs to be in bootload mode long enough to allow finishing. A slow bootloader will require longer delays.

In lockout mode, if the host is foreign both the local MCU node and Host are disconnected from the bus, otherwise, the host remains connected.


## I2C/TWI Slave

The I2C address is 0x29 (dec 41). It is organized as an array of read or write commands. Note: the sent data is used to size the reply, so add an extra byte after the command to size the reply.

0. read the shields RPU_BUS addrss and activate normal mode (boadcast if localhost_active).
1. writes this shields RPU_BUS address to eeprom (not implemented)
2. read the address sent when DTR/RTS toggles 
3. write the address that will be sent when DTR/RTS toggles
4. reads TBD (not implemented)
5. write TBD (not implemented)
6. reads error status bits[0:DTR readback timeout, 1:twi transmit fail, 2:DTR readback not match]
7. wrties (or clears) error status 


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
