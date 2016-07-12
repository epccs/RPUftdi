# Host2Local

set the bus manager on RPUftdi board as Host to Local 

## Overview

RS485 pairs have RX driver enabled and receiver enabled, TX driver enabled and receiver enabled, and FTDI_nDTR is used to control nDTR.

Basicly the USB UART outputs FTDI_TX that passes though RX transceiver to the local MCU board (e.g. Arduino Uno) RX line. While the FTDI_RX takes input from the TX transceiver that is from the local MCU board TX line. 

FTDI_nDTR signal is copied to nDTR to capacitivly reset the local MCU. DTR_DE is held low to disable the DTR pair driver. DTR_nRE is held high to disable the DTR pair receiver. 

