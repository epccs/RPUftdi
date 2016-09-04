# Host2Remote

This bus manager firmware is for an RPUftdi board, it will send a byte to reset a remote Remote MCU board (e.g. an RPUadpt on Irrigate7) when DTR toggles.

## Overview

RS485 pairs have RX driver enabled and receiver disabled, TX driver enabled and receiver disabled, and FTDI_nDTR is used to control when byte is sent on DTR_TXD.

Basicly the USB UART outputs FTDI_TX that drives the RX pair driver so it can be sent to the Remote device. While the FTDI_RX takes input from the TX pair receiver which is from the Remote device. 

FTDI_nDTR signal will cause a byte to be sent on the DTR pair to reset the Remote MCU. DTR_DE is held high to enable the DTR pair driver. DTR_nRE is held low to enable the DTR pair receiver. 

