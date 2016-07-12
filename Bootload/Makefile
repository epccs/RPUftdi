# Makefile 
# Used to set fues, the RPUftdi does not have a bootloader at this time 
TARGET = Bootload
LIBDIR = ../lib
OBJECTS = 

## The Arduino as ISP sketch is loaded in an Uno that shows as /dev/ttyACMx
##/dev/ttyACM0

all:

$(TARGET):

# sketch size: 30720 Diecimila (2k byt,bootsz 1k wrd), 32256 Uno (512 byt,bootsz 256 wrd), 32768 (nil bootsz)
# AVR instructions are essentially words, e.g. multiples of 16bits (except reading data from program memory, LPM)

# 8MHz internal, reset to addr 0x0000, preserve EEPROM, 2v7 B.O.D. Enabled

## http://eleccelerator.com/fusecalc/fusecalc.php?chip=atmega328p&LOW=E2&HIGH=DE&EXTENDED=05&LOCKBIT=3F
## stk500v1 is for Arduino as ISP (e.g. it is an example in the Arduino.cc IDE 1.6.7+)
fuse: 
	avrdude -v -p atmega328p -c stk500v1 -P /dev/ttyACM0 -b 19200 -e -U lock:w:0x3f:m -U lfuse:w:0xE2:m -U hfuse:w:0xDE:m -U efuse:w:0xFD:m

read: 
	avrdude -v -p atmega328p -c stk500v1 -P /dev/ttyACM0 -b 19200 -U lock:r:0x3f:m -U lfuse:r:0xE2:m -U hfuse:r:0xDE:m -U efuse:r:0xFD:m

 
