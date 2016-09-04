/* Host2Remote is for ATMega328 bus manager on RPUftdi board
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
#include "../lib/timers.h"
#include "../lib/uart.h"
#include "../lib/pin_num.h"
#include "../lib/pins_board.h"

#define BLINK_DELAY 500

static unsigned long blink_started_at;

int main(void)
{
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);
    pinMode(FTDI_nRTS, INPUT);
    pinMode(FTDI_nCTS, OUTPUT);
    digitalWrite(FTDI_nCTS, LOW);
    pinMode(FTDI_nDTR, INPUT);
    pinMode(FTDI_nDSR, OUTPUT);
    digitalWrite(FTDI_nDSR, LOW);
    pinMode(RX_DE, OUTPUT);
    digitalWrite(RX_DE, HIGH);  // allow RX pair driver to enable if FTDI_TX is low
    pinMode(TX_DE, OUTPUT);
    digitalWrite(TX_DE, LOW); // disallow TX pair driver to enable if TX (from MCU) is low
    pinMode(DTR_DE, OUTPUT);
    digitalWrite(DTR_DE, LOW);  // disallow DTR pair driver to enable if DTR_TXD is low
    pinMode(nSS, INPUT); // nSS is input to a Open collector buffer used to pull to MCU nRESET low
    digitalWrite(nSS, HIGH); 

    uint8_t activate_bootloader = 0;

    //Timer0 Fast PWM mode, Timer1 & Timer2 Phase Correct PWM mode.
    initTimers(); 

    /* Initialize UART, it returns a pointer to FILE so redirect of stdin and stdout works*/
    stdout = stdin = uartstream0_init(BAUD);

    sei(); // Enable global interrupts to start TIMER0 and UART

    uart0_flush(); // dump the transmit buffer
    digitalWrite(DTR_DE, HIGH);  // allow DTR pair driver to enable if DTR_TXD is low

    blink_started_at = millis();

    while (1) 
    {
        unsigned long kRuntime = 0;
        
        kRuntime = millis() - blink_started_at;
        
        if ( (kRuntime > BLINK_DELAY) & activate_bootloader)
        {
            digitalToggle(LED_BUILTIN);
            
            // next toggle 
            blink_started_at += BLINK_DELAY; 
        }
        
        if ( !digitalRead(FTDI_nDTR) )  // both FTDI_nDTR and FTDI_nRTS are set (active low) when avrdude tries to use the bootloader
        { 
            if (!activate_bootloader)
            {
                char input;
                char output = '1';

                // send a char to the UART output
                printf("%c", output); 

                // wait to read the UART input at least a BLINK_DELAY time
                blink_started_at += BLINK_DELAY;
                kRuntime = millis() - blink_started_at;
                while(!uart0_available() & (kRuntime < BLINK_DELAY))
                {
                    kRuntime = millis() - blink_started_at;
                }
                
                // perhaps the DTR line is stuck (e.g. someone else pulled it low) so we can't send data
                if (uart0_available())
                {
                    input = getchar();
                    if(input == output) 
                    {
                         activate_bootloader =1;
                    }
                }
            }
        }
        else
        {
            activate_bootloader =0;
            digitalWrite(LED_BUILTIN, HIGH);
        }
    }    
}

