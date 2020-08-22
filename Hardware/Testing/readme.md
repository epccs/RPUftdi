# Description

This is a list of Test preformed on each RPUftdi after assembly.

# Table of References


# Table Of Contents:

1. Basics
1. Assembly check
1. IC Solder Test
1. Bias USB 5V and Check LDO Regulators
1. [USB Power Up](#usb-power-up)
1. Set Fuse and load Bootloader
10. [Load i2c-debug into an MCU board](#load-i2c-debug-into-an-mcu-board)
11. [I2C Slave test](#i2c-slave-test)



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
{   "I_IN_MCU_12MHZ_LP-CRYSTAL_mA":[tbd,]}
^4
{   "I_IN_MCU_12MHZ_LP-CRYSTAL_mA":[7.3,7.3,7.2,7.1,7.1,7.2,7.1,]}
^3
{   "I_IN_MCU_8MHZ_INTRN_mA":[6.3,6.2,6.0,6.1]}
```






## Load i2c-debug into an MCU board

Plug the DUT into a ATmega328p MCU board (e.g. RPUno). Connect USB from a host computer to the RPUftdi to power the bus manager. Connect the ICSP tool (J9).

Use the command line to select the Host2Remote source working directory. Run the makefile rule used to load Host2Remote firmware.

```
cd ~RPUftdi/Host2Remote
make isp
```

Use picocom to connect, and verify that the red MNG_LED blinks. The DUT's red MNG_LED blinks when the UART pulls DTR active. Exit picocom with C-a, C-x. The MNG_LED should stop blinking after picocom exits.

```
picocom -b 38400 /dev/ttyUSB0
```

Now load the i2c-debug firmware onto the RPUno board through the DUT. Watch the yellow USBRX and red USBTX to verify they are working. 

```
cd ~RPUno/i2c-debug
make bootload
```

Remove power by pulling the USB plug. 

Note: The serial bootload has to travel out the FTDI_TX line and into the RX transceiver driver (which also enables it). The RX differential pair is thus driven and seen by all the differential receivers on the RX pair. The RX pair receiver drives the bare metal RX line when it is enabled by the bus manager. The direction data flows is changed but it works similarly from the bare metal TX line to the FTDI_RX line.


## I2C Slave test

With i2c-debug loaded in the RPUno board under the DUT hotplug the USB to the RPUftdi board. If BUILTIN_LED is connected it will blink at 0.5Hz (1 sec on, and 1 sec off) when I2C reads correctly.

Note: hotplugging can brownout power to other USB devices when the plugged device does not implement a soft start circuit, but the RPUftdi should not cause such brownouts because it has a soft start.

Note: For reference, the Host2Remote firmware uses the DTR pair to maintain a state machine. When picocom connects it will broadcast a byte on the DTR pair that other bus managers can receive. The following will use I2C command 0x03 (implemented in Host2Remote) to set the address ('1' or 0x31) that is broadcast when a host computer pulls the FTDI_nDTR active.

```
rsutherland@conversion:~/Samba/RPUftdi/CheckDTR$ picocom -b 38400 /dev/ttyUSB0
...
/0/id?
{"id":{"name":"I2Cdebug","desc":"RPUno Board /w atmega328p and LT3652","avr-gcc":"4.9"}}
/0/scan?
{"scan":[{"addr":"0x29"}]}
/0/address 41
{"address":"0x29"}
/0/buffer 3,49
{"txBuffer":["data":"0x3","data":"0x31"]}
/0/write
{"returnCode":"success"}
/0/read? 2
{"rxBuffer":[{"data":"0x3"},{"data":"0x31"}]}
```

Since there is no address '1' the RPUftdi bus manager LED will blink slow until the bootload timout is done, a power cycle will reset the bootload address to '0'.
