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
#include "../lib/twi.h"
#include "../lib/uart.h"
#include "../lib/pin_num.h"
#include "../lib/pins_board.h"

//slave address, use numbers between 0x08 to 0x78
#define TWI_ADDRESS 0x29
// 0x30 is ascii char for the letter zero, which may be used as second char of command e.g. /0/id?
#define RPU_ADDRESS '0'  
// default address sent on DTR pair when FTDI_nDTR toggles
#define RPU_ADDRESS_TO_BOOTLOAD '0' 
// return to normal mode address sent on DTR pair (haha... no it is not oFF it is a hex value)
#define RPU_NORMAL_MODE 0xFF

static uint8_t rxBuffer[TWI_BUFFER_LENGTH];
static uint8_t rxBufferLength = 0;
static uint8_t txBuffer[TWI_BUFFER_LENGTH];
static uint8_t txBufferLength = 0;

#define BLINK_BOOTLD_DELAY 500
#define BLINK_LOCKOUT_DELAY 2000
#define BLINK_ERROR_DELAY 200
#define VERIFY_UART_DELAY 500
#define LOCKOUT_DELAY 30000

static unsigned long blink_started_at;
static unsigned long lockout_started_at;
static unsigned long uart_started_at;

static uint8_t activate_bootloader;
static uint8_t bootloader_address; 
static uint8_t activate_lockout;
static uint8_t transmitted_uart;

volatile uint8_t error_status;

// called when I2C data is received. 
void receiveEvent(uint8_t* inBytes, int numBytes) 
{
    // I have to save the data
    for(uint8_t i = 0; i < numBytes; ++i)
    {
        rxBuffer[i] = inBytes[i];    
    }
    rxBufferLength = numBytes;
    
    // This will copy the data to the txBuffer, which will then echo back with slaveTransmit()
    for(uint8_t i = 0; i < numBytes; ++i)
    {
        txBuffer[i] = inBytes[i];    
    }
    txBufferLength = numBytes;
    if (txBufferLength > 1)
    {
        if ( (txBuffer[0] == 0) ) // TWI command to read RPU_ADDRESS
        {
            txBuffer[1] = RPU_ADDRESS; // '0' is 0x30
        }
        if ( (txBuffer[0] == 2) ) // read byte sent on DTR pair when FTDI_nDTR toggles
        {
            txBuffer[1] = bootloader_address;
        }
        if ( (txBuffer[0] == 3) ) // buffer the byte that is sent on DTR pair when FTDI_nDTR toggles
        {
            bootloader_address = txBuffer[1];
        }
        if ( (txBuffer[0] == 5) ) // send RPU_NORMAL_MODE on DTR pair
        {
            if ( !(transmitted_uart) )
            {
                // send a byte to the UART output
                uart_started_at = millis();
                printf("%c", RPU_NORMAL_MODE); 
                transmitted_uart = 1;
            }
        }
        if ( (txBuffer[0] == 6) ) // TWI command to read error status
        {
            txBuffer[1] = error_status;
        }
        if ( (txBuffer[0] == 7) ) // TWI command to set/clear error status
        {
            error_status = txBuffer[1];
        }
    }
}

// called when I2C data is requested.
void slaveTransmit(void) 
{
    // respond with an echo of the last message sent
    uint8_t return_code = twi_transmit(txBuffer, txBufferLength);
    if (return_code != 0)
        error_status &= (1<<1); // bit one set 
}


void setup(void) 
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
    pinMode(RX_nRE, OUTPUT);
    digitalWrite(RX_nRE, LOW);  // enable RX pair recevior to output to local MCU's RX input
    pinMode(TX_DE, OUTPUT);
    digitalWrite(TX_DE, HIGH); // allow TX pair driver to enable if TX (from MCU) is low
    pinMode(TX_nRE, OUTPUT);
    digitalWrite(TX_nRE, LOW);  // enable TX pair recevior to output to FTDI_RX input
    pinMode(DTR_DE, OUTPUT);
    digitalWrite(DTR_DE, LOW);  // disallow DTR pair driver to enable if DTR_TXD is low
    pinMode(nSS, OUTPUT); // nSS is input to a Open collector buffer used to pull to MCU nRESET low
    digitalWrite(nSS, HIGH); 

    bootloader_address =RPU_ADDRESS_TO_BOOTLOAD; 
    activate_bootloader = 0;
    activate_lockout = 0;
    transmitted_uart = 0;
    error_status = 0;

    //Timer0 Fast PWM mode, Timer1 & Timer2 Phase Correct PWM mode.
    initTimers(); 

    /* Initialize UART, it returns a pointer to FILE so redirect of stdin and stdout works*/
    stdout = stdin = uartstream0_init(BAUD);

    twi_setAddress(TWI_ADDRESS);
    twi_attachSlaveTxEvent(slaveTransmit); // called when I2C data is requested 
    twi_attachSlaveRxEvent(receiveEvent); // slave receive
    twi_init(false); // do not use internal pull-up

    sei(); // Enable global interrupts to start TIMER0 and UART

    uart0_flush(); // dump the transmit buffer
    digitalWrite(DTR_DE, HIGH);  // allow DTR pair driver to enable if DTR_TXD is low
}

// blink if the bootloader has been activated or fast blink if error status is set
void blink_on_activate(void)
{
    unsigned long kRuntime = millis() - blink_started_at;
    
    if (!error_status) 
    {
        if ( activate_bootloader  && (kRuntime > BLINK_BOOTLD_DELAY) )
        {
            digitalToggle(LED_BUILTIN);
            
            // next toggle 
            blink_started_at += BLINK_BOOTLD_DELAY; 
        }
        if ( activate_lockout  && (kRuntime > BLINK_LOCKOUT_DELAY) )
        {
            digitalToggle(LED_BUILTIN);
            
            // next toggle 
            blink_started_at += BLINK_LOCKOUT_DELAY; 
        }
    }
    else
    {
        if ( (kRuntime > BLINK_ERROR_DELAY))
        {
            digitalToggle(LED_BUILTIN);
            
            // next toggle 
            blink_started_at += BLINK_ERROR_DELAY; 
        }
    }
}

void check_DTR(void)
{
    if ( !digitalRead(FTDI_nDTR) )  // both FTDI_nDTR and FTDI_nRTS are set (active low) when avrdude tries to use the bootloader
    { 
        if ( !(activate_bootloader || activate_lockout || transmitted_uart) )
        {
            // send a byte to the UART output
            uart_started_at = millis();
            printf("%c", bootloader_address); 
            transmitted_uart = 1;
        }
    }
    else
    {
        activate_bootloader =0;
        digitalWrite(LED_BUILTIN, HIGH);
    }
}

// lockout needs to happoen for a long enough time to insure bootloading is finished,
void check_lockout(void)
{
    unsigned long kRuntime = millis() - lockout_started_at;
    
    if ( activate_lockout && (kRuntime > LOCKOUT_DELAY))
    {
        activate_lockout =0;
        digitalWrite(LED_BUILTIN, HIGH);
        digitalWrite(RX_DE, HIGH); // allow RX pair driver to enable if FTDI_TX is low
        digitalWrite(RX_nRE, LOW);  // enable RX pair recevior to output to local MCU's RX input
        digitalWrite(TX_DE, HIGH); // allow TX pair driver to enable if TX (from MCU) is low
        digitalWrite(TX_nRE, LOW);  // enable TX pair recevior to output to FTDI_RX input
        
        // set bootloader mode to prevent check_DTR from looping into anogher lockout
        activate_bootloader = 1;
    }
}

void check_uart(void)
{
    unsigned long kRuntime = millis() - uart_started_at;
 
   if ( uart0_available() )
    {
        char input;
        input = getchar();

        if (input == RPU_NORMAL_MODE)
        { // end the lockout if it was set.
            activate_lockout =0;
            activate_bootloader =0;
            digitalWrite(LED_BUILTIN, HIGH);
            digitalWrite(RX_DE, HIGH); // allow RX pair driver to enable if FTDI_TX is low
            digitalWrite(RX_nRE, LOW);  // enable RX pair recevior to output to local MCU's RX input
            digitalWrite(TX_DE, HIGH); // allow TX pair driver to enable if TX (from MCU) is low
            digitalWrite(TX_nRE, LOW);  // enable TX pair recevior to output to FTDI_RX input
        }
        else
        {
            if (input == RPU_ADDRESS) // that is my local address, so I should reset my MCU and keep RX/TX open
            {
                activate_bootloader =1;
                digitalWrite(nSS, LOW);   // nSS goes through a open collector buffer to nRESET
                _delay_ms(20);  // hold reset low for a short time 
                digitalWrite(nSS, HIGH); // this will release the buffer with open colllector on MCU nRESET.
            }
            else
            {  // lockout MCU, but do not block the ftdi host
                activate_lockout =1;
                digitalWrite(RX_DE, LOW); // allow RX pair driver to enable if FTDI_TX is low
                digitalWrite(RX_nRE, HIGH);  // disable RX pair recevior to output to local MCU's RX input
                digitalWrite(TX_DE, LOW); // disallow TX pair driver to enable if TX (from MCU) is low
                digitalWrite(TX_nRE, HIGH);  // enable TX pair recevior to output to FTDI_RX input
                lockout_started_at = millis();
                blink_started_at = millis();
            }
        }

        if ( transmitted_uart )
        {
            if(input != bootloader_address) 
            { // sent byte does not match,  but I'm not to sure what would cause this.
                error_status &= (1<<2); // bit two set 
            }
            transmitted_uart = 0;
         }
    }
    else if (transmitted_uart && (kRuntime > VERIFY_UART_DELAY) )
    { // perhaps the DTR line is stuck (e.g. someone has pulled it low) so we time out
        error_status &= (1<<0); // bit zero set 
        transmitted_uart = 0;
    }
}


int main(void)
{
    setup();

    blink_started_at = millis();

    while (1) 
    {
        blink_on_activate();
        check_DTR();
        check_lockout();
        check_uart();
    }    
}

