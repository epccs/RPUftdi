# Host2Remote

set the bus manager on RPUftdi board as Host to Remote 

## Overview

RS485 pairs have RX driver enabled and receiver disabled, TX driver enabled and receiver disabled, and FTDI_nDTR is used to control DTR_TXD.

Basicly the USB UART outputs FTDI_TX that drives the RX pair driver so it can be sent to the Remote device. While the FTDI_RX takes input from the TX pair receiver which is from the Remote device. 

FTDI_nDTR signal is copied to FTDI_nDTR to reset the Remote MCU. DTR_DE is held high to enable the DTR pair driver. DTR_nRE is held low to enable the DTR pair receiver. 

