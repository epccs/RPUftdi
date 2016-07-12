/* Blink on RPUftdi 
 * blink the LED and toggle some IO for testing.
 */ 

#include <util/delay.h>
#include <avr/io.h>
#include "../lib/timers.h"
#include "../lib/pin_num.h"
#include "../lib/pins_board.h"

static unsigned long blink_started_at;

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
    digitalWrite(DTR_nRE, LOW); // enable DTR pair receiver
    pinMode(DTR_DE, OUTPUT);
    digitalWrite(DTR_DE, HIGH); // enable DTR pair driver
    pinMode(DTR_TXD, OUTPUT);
    digitalWrite(DTR_TXD, LOW); // transmit a false to DTR_TX so that DTR_DE can toggle the DTR driver.
    pinMode(SS, INPUT); // warning SS is connecte to Arduino nRESET
    
    initTimers(); //Timer0 Fast PWM mode, Timer1 & Timer2 Phase Correct PWM mode.

    sei(); // Enable global interrupts to start TIMER0

    while (1) 
    {
        unsigned long kRuntime = 0;
        
        digitalWrite(LED_BUILTIN, LOW);      // turn the LED on by sinking current
        blink_started_at = millis();
        while (kRuntime <= 100) // wait for 1/10 second
        {
            kRuntime = millis() - blink_started_at;
        }
        digitalToggle(LED_BUILTIN);       // toggle the LED off
        digitalWrite(FTDI_nCTS, !digitalRead(FTDI_nCTS));  // toggle CTS for DMM
        digitalWrite(FTDI_nDSR, !digitalRead(FTDI_nDSR));  // toggle DSR for DMM
        digitalWrite(DTR_DE, !digitalRead(DTR_DE));  // toggle DTR driver, and watch input current
        _delay_ms(1900);              // wait for 19/10 second
    }    
}

