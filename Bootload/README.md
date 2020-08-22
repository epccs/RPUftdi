# Serial Bootloader

## Overview

AVR microcontrollers are usually programmed through an In-circuit Serial Programmer (ICSP/ISP), however with this firmware loaded in your microcontroller programing can be done over a serial interface (TX/RX) without the ICSP programmer. This is called serial bootloading. 

Unfortunalty to install a serial bootloader requires an ICSP tool. Arduino has an [ArduinoISP] sketch that I load into a Arduino Uno to use as an ICSP tool.

[ArduinoISP]: https://github.com/arduino/Arduino/blob/master/build/shared/examples/11.ArduinoISP/ArduinoISP/ArduinoISP.ino

It is a two step process. First run "make fuse" which will unlock the boot flash section and set fuses (LOW, HIGH, EXTENDED). The changed fuse setting will switch operation form the internal oscilator to the exteranl crystal, if it is missing the MCU will be locked up. The second step is "make isp" which will load the bootloader and lock the boot flash section, which should prohibit the Store Program Memory (SPM) instruction from running in the boot flash section. 

    WARNING: programing these fuse valuse will lock up the MCU when the crystal is missing or damaged.


## License 

The main.c is GPL while boot.h is BSD (since the code is the documentation just read the license in it).


## Firmware Upload

With ISP tool connected.

``` 
sudo apt-get install make git gcc-avr binutils-avr gdb-avr avr-libc avrdude
git clone https://github.com/epccs/RPUusb/Bootloader
cd /RPUno/Bootloader
make all
make isp
...
avrdude -v -p atmega328pb -C +../lib/avrdude/328pb.conf -c stk500v1 -P /dev/ttyACM0 -b 19200 -U eeprom:r:optiboot_rpuusb_atmega328pb_eeprom.hex:i
...
avrdude: safemode: Fuses OK (E:FD, H:DE, L:FF)
...
avrdude -v -p atmega328pb -C +../lib/avrdude/328pb.conf -c stk500v1 -P /dev/ttyACM0 -b 19200 -e -U lock:w:0xFF:m -U lfuse:w:0xFF:m -U hfuse:w:0xDE:m -U efuse:w:0xFD:m
...
avrdude: safemode: Fuses OK (E:FD, H:DE, L:FF)
...
avrdude -v -p atmega328pb -C +../lib/avrdude/328pb.conf -c stk500v1 -P /dev/ttyACM0 -b 19200 -e -U flash:w:optiboot_rpuusb_atmega328pb_-b38400UL_F_CPU12000000UL.hex -U lock:w:0xEF:m
...
avrdude done.  Thank you.
``` 

The bootloader is from MCUdude
https://github.com/MCUdude/optiboot_flash

