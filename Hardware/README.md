# Hardware

## Overview

USB interface that plugs in place of an R-Pi. It plugs into an R-Pi header (e.g., [Gravimetric]). The CP2105 chip has two UARTs. The full-duplex multidrop bus is directly connected to the primary UART, while the secondary UART is connected to an MCU (ATmega328pb) that can act as a bridge for many of the R-Pi header pins (e.g., SPI, I2C) from the target board. A USB host can set the bootload address on the multi-drop serial with the I2C interface from the 328pb, and some other functions that an R-Pi has access to from the manager.

[Gravimetric]: https://github.com/epccs/Gravimetric


## Inputs/Outputs/Functions

* Full Duplex TX and RX from USB serial0 on R-Pi header pin 8 and 10.
* RTS and CTS from USB serial0 on R-Pi header pin 36 and 11.
* USB serial1 connects with an ATmega328pb for use with I2C/SPI.
* Fits RPUpi or Gravimetric.

## Uses

* Plugs in place of an R-Pi to allow USB from a computer to communicate with an RPU target.


# Table Of Contents

1. [Status](#status)
2. [Design](#design)
3. [Bill of Materials](#bill-of-materials)
4. [How To Use](#how-to-use)


# Status

![Status](./status_icon.png "RPUusb Status")

```
        ^5  Done: Design, Layout, BOM, Review*,
            WIP: Order Boards,
            Todo: Assembly, Testing, Evaluation.
            *during review the Design may change without changing the revision.
            # remove arduino headers
            # add header with 40 pin R-Pi pinout
            # swap MCU to ATmega328pb
            # swap ftdi with a dual UART port version Silabs CP2105
            # connect serial0 to RX/TX/nRTS/nCTS on R-Pi header
            # connect sierial1 to MCU's serial0
            # connect MCU's I2C0 to I2C on R-Pi header
            # connect MCU's I2C1 to the ID port on R-Pi header
            # connect MCU's SPI to SPI on R-Pi header
            # power with 3V3 from LDO and power the LDO from the USB header through the P-CH soft start. 

        ^4 Done: Design, Layout, BOM, Review*, Order Boards, Assembly, Testing, Evaluation.
           WIP: 
           Todo:  
           *during review the Design may change without changing the revision.
           location: 2017-3-17 Test Bench /w an OSEPP Uno R3

        ^3 location: 2016-10-1 Test Bench /w an OSEPP Uno R3, used to connect by CAT5 to outside parts and to bench parts as needed.
                     2017-1-5 ICP1 hacked open.
                     2017-3-17 scraped
```

Debugging and fixing problems i.e. [Schooling](./Schooling/)

Setup and methods used for [Evaluation](./Evaluation/)


# Design

The board is 0.063 thick, FR4, two layer, 1 oz copper with ENIG (gold) finish.

![Top](./Documents/14145,Top.png "RPUusb Top")
![TAssy](./Documents/14145,TAssy.jpg "RPUusb Top Assy")
![Bottom](./Documents/14145,Bottom.png "RPUusb Bottom")
![BAssy](./Documents/14145,BAssy.jpg "RPUusb Bottom Assy")


## Mounting

```
        Plugs into the R-Pi header of a multi-drop serial board.
```

## Electrical Schematic

![Schematic](./Documents/14145,Schematic.png "RPUusb Schematic")

## Testing

Check correct assembly and function with [Testing](./Testing/)



# Bill of Materials

The BOM is a CVS file, import it into a spreadsheet program like LibreOffice Calc (or Excel), or use a text editor.

Option | BOM's included
----- | ----- 
A. | [BRD] 
M. | [BRD] [SMD] [HDR] 


[BRD]: ./Design/14145BRD,BOM.csv
[SMD]: ./Design/14145SMD,BOM.csv
[HDR]: ./Design/14145HDR,BOM.csv

[Order Form](https://rpubus.org/Order_Form.html)


# How To Use

TDD
