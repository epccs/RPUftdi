# RPUftdi shield for RS-422 over CAT5 with USB for a host

![Status](https://raw.githubusercontent.com/epccs/RPUftdi/master/Hardware/status_icon.png "Status")

From <https://github.com/epccs/RPUftdi>

## Overview

Shield used to connect a node and a USB host to a RPU_BUS, which is multidrop full duplex RS-422 (RX and TX pairs) with an out of band half duplex RS-485 (DTR pair).

![Schematic](https://raw.githubusercontent.com/epccs/RPUftdi/master/Hardware/14145,Schematic.png "RPUftdi Schematic")

![Bottom](https://raw.githubusercontent.com/epccs/RPUftdi/master/Hardware/14145,Bottom.png "RPUftdi Board Bottom")

![Top](https://raw.githubusercontent.com/epccs/RPUftdi/master/Hardware/14145,Top.png "RPUftdi Board Top")

[HackADay](https://hackaday.io/project/15585-rpuftid).

[Forum](http://rpubus.org/bb/viewforum.php?f=5).

The core files are in the /lib folder while each example has its own Makefile.

## AVR toolchain

    * sudo apt-get install [gcc-avr](http://packages.ubuntu.com/search?keywords=gcc-avr)
    * sudo apt-get install [binutils-avr](http://packages.ubuntu.com/search?keywords=binutils-avr)
    * sudo apt-get install [gdb-avr](http://packages.ubuntu.com/search?keywords=gdb-avr)
    * sudo apt-get install [avr-libc](http://packages.ubuntu.com/search?keywords=avr-libc)
    * sudo apt-get install [avrdude](http://packages.ubuntu.com/search?keywords=avrdude)