# Description

This is a list of Test preformed on each RPUftdi after assembly.

# Table of References


# Table Of Contents:

1. Basics
1. Assembly check
1. IC Solder Test
1. Bias USB 5V and Check LDO Regulators
1. USB Power Up
1. Set Fuse and load Bootloader
1. Load i2c-debug into the MCU at secondary UART




## Basics

These tests are for an assembled RPUftdi board 14145^5 which may be referred to as a Unit Under Test (UUT). If the UUT fails and can be reworked then do so, otherwise it needs to be scraped. 

**Warning: never use a soldering iron to rework ceramic capacitors due to the thermal shock.**
    
Items used for test.

![ItemsUsedForTest](./14145,ItemsUsedForTest.jpg "RPUusb Items Used For Test")


## Assembly check

After assembly check the circuit carefully to make sure all parts are soldered and correct, note that the device making is labeled on the schematic and assembly drawing.
    

## IC Solder Test

Check continuity between pin and pad by measuring the reverse body diode drop from 0V (aka ground) and all other IC pads not connected to 0V. This value will vary somewhat depending on what the pin does, but there is typically an ESD diode to ground or sometimes a body diode (e.g. open drain MOSFET), thus a value of .3V to .7V is valid to indicate a solder connection. Note the RS485 drivers will show high impedance on the differential lines, so skip those.


## Bias USB 5V and Check LDO Regulators

Connect a 5V supply with CC mode set at 30mA to the USB input. Verify that the 5V gets through the fuse and soft start circuit to the +5V pin. Turn off power. 

Connect a 5V supply with CC mode set at 30mA to the the USB +5V input. Measure that the the MIC5205 (U2) linear regulator has 3.3V (J2 pin 1). Measure the input current of a blank MCU. Turn off power.

```
{   "I_IN_BLANKMCU_mA":[1.44,],
    "LDO_V":[3.41,] }
^4
{   "I_IN_BLANKMCU_mA":[4.5,4.5,4.3,4.4,],
    "LDO_V":[3.3,3.3,3.28,3.28,] }
```


## USB Power Up

On Linux use the dmesg command to see latest device mesages then plug the USB input to a powered hub that was previously connected and use dmesg to verify that the new USB port shows up.

```
dmesg
...
[11543.622763] usb 2-2: new full-speed USB device number 4 using uhci_hcd
[11543.806214] usb 2-2: New USB device found, idVendor=10c4, idProduct=ea70, bcdDevice= 1.00
[11543.806217] usb 2-2: New USB device strings: Mfr=1, Product=2, SerialNumber=5
[11543.806220] usb 2-2: Product: CP2105 Dual USB to UART Bridge Controller
[11543.806222] usb 2-2: Manufacturer: Silicon Labs
[11543.806224] usb 2-2: SerialNumber: 00B3EDE8
[11543.871127] usbcore: registered new interface driver cp210x
[11543.872699] usbserial: USB Serial support registered for cp210x
[11543.873601] cp210x 2-2:1.0: cp210x converter detected
[11543.881730] usb 2-2: cp210x converter now attached to ttyUSB0
[11543.882017] cp210x 2-2:1.1: cp210x converter detected
[11543.889340] usb 2-2: cp210x converter now attached to ttyUSB1
[38868.129478] usb 2-2: USB disconnect, device number 4
[38868.129765] cp210x ttyUSB0: cp210x converter now disconnected from ttyUSB0
[38868.133806] cp210x 2-2:1.0: device disconnected
[38868.134540] cp210x ttyUSB1: cp210x converter now disconnected from ttyUSB1
[38868.134679] cp210x 2-2:1.1: device disconnected
[38890.722140] usb 2-2: new full-speed USB device number 5 using uhci_hcd
[38890.905536] usb 2-2: New USB device found, idVendor=10c4, idProduct=ea70, bcdDevice= 1.00
[38890.905539] usb 2-2: New USB device strings: Mfr=1, Product=2, SerialNumber=5
[38890.905542] usb 2-2: Product: CP2105 Dual USB to UART Bridge Controller
[38890.905544] usb 2-2: Manufacturer: Silicon Labs
[38890.905546] usb 2-2: SerialNumber: 00B3EDE8
[38890.911642] cp210x 2-2:1.0: cp210x converter detected
[38890.918612] usb 2-2: cp210x converter now attached to ttyUSB0
[38890.920600] cp210x 2-2:1.1: cp210x converter detected
[38890.927608] usb 2-2: cp210x converter now attached to ttyUSB1
...
```


## Set MCU Fuse

Install Git and AVR toolchain on Ubuntu (20.04). 

```
sudo apt-get install git gcc-avr binutils-avr gdb-avr avr-libc avrdude
```

Clone the RPUftdi repository.

```
cd ~
git clone https://github.com/epccs/RPUusb
cd RPUusb/Bootload
```

Connect a USB host to supply power. Connect the ICSP tool (J3). The MCU needs its fuses set, so run the Makefile rule to do that. 

```
make fuse
```

Next load the bootloader

```
# a copy of the hex file is in the repo
[make all]
make isp
```


Disconnect the ICSP tool and measure the input current for 12Mhz crystal at 3.3V. It takes a long time to settel.

```
{   "I_IN_MCU_12MHZ-CRYSTAL_mA":[8.9,]}
^4
{   "I_IN_MCU_12MHZ_LP-CRYSTAL_mA":[7.3,7.3,7.2,7.1,7.1,7.2,7.1,]}
^3
{   "I_IN_MCU_8MHZ_INTRN_mA":[6.3,6.2,6.0,6.1]}
```

Some MCU pins are floating.



## Load i2c-debug into the MCU at secondary UART

Plug the DUT into a Gravimetric board (or other simular applicaion board). Power the application board first, then connect the USB from a host computer to the RPUusb.

Warning: powering the RPUusb first will damage the manager on the application board (unless I figure out how to use an AVR-DB for the manager, which has dual power domains.)

Use the command line to upload the i2c-debug firmware after building it.

```
cd ~RPUusb/i2c-debug
make all
make bootload
```

Use picocom to connect. Exit picocom with C-a, C-x.

```
picocom -b 38400 /dev/ttyUSB1
...
Terminal ready
i2c-debug at addr 48: commands include
/0/id?
...
/0/id?
{"id":{"name":"I2Cdebug^2","desc":"RPUusb (14145^5) Board /w ATmega328pb","avr-gcc":"5.4.0"}}
```

Scan for the manager on the applicaion board. It is on I2C0 of the MCU we are talking to on USB UART1.

```
/0/iscan?
{"scan":[{"addr":"0x2A"}]}
```



