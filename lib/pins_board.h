/*
 * Pin definitions for use with DigitalIO library
 * Copyright (C) 2016 Ronald Sutherland
 *
 * This Library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This Library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License 
 * along with the Arduino DigitalIO Library.  If not, see
 * <http://www.gnu.org/licenses/>.
 */
#ifndef Pins_Board_h
#define Pins_Board_h

/* 
    32TQFPpin
        ATTMEGA328Preg-bit
              {PCINT}
                     (ArduinoDigital)
                            function #notes [RPUftdi]
    12 PB0 {0} (D 8) ICP1 [!DTR!]
    13 PB1 {1} (D 9) OC1A [LED_BUILTIN]
    14 PB2 {2} (D 10) SS OC1B [SS]
    15 PB3 {3} (D 11) MOSI OC2A [MOSI]
    16 PB4 {4} (D 12) MISO [MISO]
    17 PB5 {5} (D 13) SCK [SCK]
    23 PC0 {8} (D 14) ADC0 [FTDI_!CTS!]
    24 PC1 {9} (D 15) ADC1 [FTDI_!DSR!]
    25 PC2 {10} (D 16) ADC2 [TX_!RE!]
    26 PC3 {11} (D 17) ADC3 [TX_DE]
    27 PC4 {12} (D 18) ADC4 SDA [SDA0]
    28 PC5 {13} (D 19) ADC5 SCL [SCL0]
    19 (D 20) ADC6 [USB5V_ADC6]
    22 (D 21) ADC7 [VIN_ADC7]
    9 PD0 {16} (D 0) RXD [DTR_RXD]
    8 PD1 {17} (D 1) TXD [DTR_TXD]
    7 PD2 {18} (D 2) INT0 [FTDI_!DTR!]
    6 PD3 {19} (D 3) INT1 OC2B [FTDI_!RTS!]
    5 PD4 {20} (D 4) T0 [RX_!RE!]
    4 PD5 {21} (D 5) T1 OC0B [TX_DE]
    3 PD6 {22} (D 6) OC0A [DTR_!RE!]
    2 PD7 {23} (D 7) [DTR_DE]
*/

#define NUM_DIGITAL_PINS            20
#define NUM_ANALOG_INPUTS        8
// analogInputToDigitalPin() takes an AVR analog channel number and returns the digital pin number otherwise -1.
//#define analogInputToDigitalPin(p)  ((p < 8) ? (p) + 14 : -1)
#define digitalPinHasPWM(p)         ((p) == 3 || (p) == 5 || (p) == 6 || (p) == 9 || (p) == 10 || (p) == 11)

static const uint8_t DTR_RXD = 0; // testing only, not used /w Serial.available() then Serial.Read()
static const uint8_t DTR_TXD = 1; // testing only, not used /w Serial.print("stuf")
static const uint8_t FTDI_nDTR = 2; // e.g. ISR(INT0_vect) see http://www.nongnu.org/avr-libc/user-manual/group__avr__interrupts.html
static const uint8_t FTDI_nRTS = 3; // e.g. ISR(INT1_vect)
static const uint8_t RX_nRE = 4; // a True will disable RX pair receiver
static const uint8_t TX_DE = 5; // a True will enable TX pair driver
static const uint8_t DTR_nRE = 6; // a True will disable DTR pair receiver
static const uint8_t DTR_DE = 7; // a True will enable DTR pair driver
static const uint8_t nDTR = 8; // toggle to capacitivly reset the bare metal into bootloader.

static const uint8_t LED_BUILTIN = 9;

static const uint8_t SS   = 10; 
static const uint8_t MOSI = 11;
static const uint8_t MISO = 12;
static const uint8_t SCK  = 13;

static const uint8_t FTDI_nCTS = 14; // output to FTDI chip
static const uint8_t FTDI_nDSR = 15; // output to FTDI chip
static const uint8_t TX_nRE = 16; // a True will disable TX pair receiver
static const uint8_t RX_DE = 17; // a True will enable RX pair driver

static const uint8_t SDA = 18;
static const uint8_t SCL = 19;

// these are ADC channels, they do not have digital IO function on ATmega328p
static const uint8_t USB5V_ADC6 = 6; // measure USB 5V at about 9.9mV/V
static const uint8_t VIN_ADC7 = 7; // measure VIN at about 4.98mV/V

#endif // Pins_Board_h
