# TWI/I2C slave 

Like Host2Local but add I2C slave that will echo

## Overview

See Host2Local for bus notes.

The I2C will echo the bytes, so just send some data and read it back.

# Testing

To see how it works I plug the RPUftdi shield onto a RPUno and load the i2c-debug progam.

Now connect with picocom (or ilk).

``` 
#exit picocom with C-a, C-x
picocom -b 115200 /dev/ttyUSB0
``` 

Try the following commands and check that the JSON is as expected (The I2C slave will echo the bytes).

``` 
/0/id?
{"id":{"name":"I2Cdebug","desc":"RPUno Board /w atmega328p and LT3652","avr-gcc":"4.9"}}
/0/scan?
{"scan":[{"addr":"0x29"}]}
/0/address 41
{"address":"0x29"}
/0/buffer 3,0
{"txBuffer":["data":"0x3","data":"0x0"]}
/0/write
{"returnCode":"success"}
/0/read? 2
{"rxBuffer":[{"data":"0x3"},{"data":"0x0"}]}
/0/buffer 45,253
{"txBuffer":[{"data":"0x2D"},{"data":"0xFD"}]}
/0/read? 2
{"rxBuffer":[{"data":"0x2D"},{"data":"0xFD"}]}
``` 

The second read sends the buffer as command bytes and then uses a repeated start to read the echo.