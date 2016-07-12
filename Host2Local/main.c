/* Host2Local is for ATMega328 bus manager on RPUftdi board
 This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

For a copy of the GNU General Public License use
http://www.gnu.org/licenses/gpl-2.0.html
 */ 

#include <util/delay.h>
#include <avr/io.h>
#include "../lib/pin_num.h"
#include "../lib/pins_board.h"

int main(void)
{
    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(FTDI_nRTS, INPUT);
    pinMode(FTDI_nCTS, OUTPUT);
    digitalWrite(FTDI_nCTS, LOW);
    pinMode(FTDI_nDTR, INPUT);
    pinMode(FTDI_nDSR, OUTPUT);
    digitalWrite(FTDI_nDSR, LOW);
    pinMode(RX_nRE, OUTPUT);
    digitalWrite(RX_nRE, LOW); // enable RX pair receiver
    pinMode(RX_DE, OUTPUT);
    digitalWrite(RX_DE, HIGH); // enable RX pair driver
    pinMode(TX_nRE, OUTPUT);
    digitalWrite(TX_nRE, LOW); // enable TX pair receiver
    pinMode(TX_DE, OUTPUT);
    digitalWrite(TX_DE, HIGH); // enable TX pair driver
    pinMode(DTR_nRE, OUTPUT);
    digitalWrite(DTR_nRE, HIGH); // disable DTR pair receiver
    pinMode(DTR_DE, OUTPUT);
    digitalWrite(DTR_DE, LOW); // disable DTR pair driver
    pinMode(DTR_TXD, OUTPUT);
    digitalWrite(DTR_TXD, HIGH); // transmit a true on DTR_TX pair, which is default for serial.
    pinMode(SS, INPUT); // warning SS is connecte to Arduino nRESET
    pinMode(nDTR, OUTPUT);
    digitalWrite(nDTR, HIGH); // start with a high and look for falling edge on FTDI_nDTR
    
    while (1) 
    {
        if ( !digitalRead(FTDI_nDTR) )         // both FTDI_nDTR and FTDI_nRTS work for this and are active low
        { 
            digitalWrite(LED_BUILTIN, LOW);      // turn the LED on by sinking current
            digitalWrite(nDTR, LOW);             // copy to nDTR
        }
        else
        {
            digitalWrite(LED_BUILTIN, HIGH);      // turn the LED off
            digitalWrite(nDTR, HIGH);             // copy to nDTR
        }
    }    
}

