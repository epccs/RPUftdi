/* IO Enumeration
Copyright (C) 2020 Ronald Sutherland

Permission to use, copy, modify, and/or distribute this software for any purpose with or without fee is hereby granted.

THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES 
WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF 
MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE 
FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY 
DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, 
WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, 
ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

https://en.wikipedia.org/wiki/BSD_licenses#0-clause_license_(%22Zero_Clause_BSD%22)

Editors like VScode with IntelliSense can show appropriate options.
Some compilers can reject code that is an incorrect type.

Some referance material to help get the needed SBI/CBI instructions
https://www.avrfreaks.net/forum/c-gpio-class-inheritance-problems
https://github.com/greiman/SdFat-beta/blob/master/src/DigitalIO/DigitalPin.h
*/

#ifndef IO_Enum_h
#define IO_Enum_h

#include <stdbool.h>
#include <stdint.h>
#include <avr/io.h>
#include <util/atomic.h>

#if defined(__AVR_ATmega328PB__)

// Direction is used to program the IO as an input or output
typedef enum DIRECTION_enum {
    DIRECTION_INPUT,
    DIRECTION_OUTPUT
} DIRECTION_t;

// Logic Level
typedef enum LOGIC_LEVEL_enum {
    LOGIC_LEVEL_LOW,
    LOGIC_LEVEL_HIGH
} LOGIC_LEVEL_t;

// enumeraiton names for the MCU_IO_<node> is on schematic
typedef enum MCU_IO_enum {
    MCU_IO_MISO1,  // PC0 goes to where R-Pi MISO1 (SPI1) would be
    MCU_IO_SCK1, // PC1 goes to where R-Pi SCK1 (SPI1) would be
    MCU_IO_BMC23, // PC2 goes to where R-Pi BMC23 would be
    MCU_IO_BMC24, // PC3 goes to where R-Pi BMC24 would be
    MCU_IO_SDA, // PC4 goes to where R-Pi SDA (I2C0) would be
    MCU_IO_SCL, // PC5 goes to where R-Pi SCL (I2C0) would be
    MCU_IO_nCE10, // PC5 goes to where R-Pi nCE10 (SPI1) would be
    MCU_IO_MOSI1, // PE3 goes to where R-Pi MOSI1 (SPI1) would be
    MCU_IO_BMC5, // PB0 goes to where R-Pi BMC5 would be
    MCU_IO_BMC26, // PB1 goes to where R-Pi BMC26 would be
    MCU_IO_nCE00, // PB2 goes to where R-Pi nCE00 (SPI0) would be
    MCU_IO_MOSI,  // PB3 goes to where R-Pi MOSI (SPI0) would be
    MCU_IO_MISO, // PB4 goes to where R-Pi MISO (SPI0) would be
    MCU_IO_SCK,   // PB5 goes to where R-Pi SCK (SPI0) would be
    MCU_IO_USB_UART1_RX1,   // PD0 goes to the secondary UART on CP2105.
    MCU_IO_USB_UART1_TX1,   // PD1 goes to the secondary UART on CP2105.
    MCU_IO_SHUTDOWN, // PD2 goes to where R-Pi BMC6 would be, which I use for a shutdown signal
    MCU_IO_BMC44, // PD3 goes to where R-Pi BMC44 would be
    MCU_IO_BMC27, // PD4 goes to where R-Pi BMC27 would be
    MCU_IO_BMC13_PWM1, // PD5 goes to where R-Pi BMC13 (PWM1) would be
    MCU_IO_BMC12_PWM0, // PD6 goes to where R-Pi BMC12 (PWM0) would be
    MCU_IO_BMC22, // PD7 goes to where R-Pi BMC22 would be
    MCU_IO_ID_SD, // PE0 goes to where R-Pi BMC0 (ID_SD) would be
    MCU_IO_ID_SC, // PE1 goes to where R-Pi BMC1 (ID_SC) would be
    MCU_IO_END // enum starts at zero, this is one past end of list for use as array size
} MCU_IO_t;

typedef volatile uint8_t register8_t;

struct IO_Map {
// https://yarchive.net/comp/linux/typedefs.html
  register8_t* ddr; // data direction register
  register8_t* in;  // input register
  register8_t* port; // output register
  uint8_t mask;  
};


// map of direct port manipulation registers used for IO functions.
const static struct IO_Map ioMap[MCU_IO_END] = 
{
    [MCU_IO_MISO1] = { .ddr=&DDRC, .in=&PINC, .port=&PORTC, .mask= 1<<PC0 },
    [MCU_IO_SCK1] = { .ddr=&DDRC, .in=&PINC, .port=&PORTC, .mask= 1<<PC1 },
    [MCU_IO_BMC23] = { .ddr=&DDRC, .in=&PINC, .port=&PORTC, .mask= 1<<PC2 },
    [MCU_IO_BMC24] = { .ddr=&DDRC, .in=&PINC, .port=&PORTC, .mask= 1<<PC3 },
    [MCU_IO_SDA] = { .ddr=&DDRC, .in=&PINC, .port=&PORTC, .mask= 1<<PC4 },
    [MCU_IO_SCL] = { .ddr=&DDRC, .in=&PINC, .port=&PORTC, .mask= 1<<PC5 },
    [MCU_IO_nCE10] = { .ddr=&DDRE, .in=&PINE, .port=&PORTE, .mask= 1<<PE2 },
    [MCU_IO_MOSI1] = { .ddr=&DDRE, .in=&PINE, .port=&PORTE, .mask= 1<<PE3 },
    [MCU_IO_BMC5] = { .ddr=&DDRB, .in=&PINB, .port=&PORTB, .mask= 1<<PB0 }, 
    [MCU_IO_BMC26] = { .ddr=&DDRB, .in=&PINB, .port=&PORTB, .mask= 1<<PB1 },
    [MCU_IO_nCE00] = { .ddr=&DDRB, .in=&PINB, .port=&PORTB, .mask= 1<<PB2 },
    [MCU_IO_MOSI] = { .ddr=&DDRB, .in=&PINB, .port=&PORTB, .mask= 1<<PB3 },
    [MCU_IO_MISO] = { .ddr=&DDRB, .in=&PINB, .port=&PORTB, .mask= 1<<PB4 },
    [MCU_IO_SCK] = { .ddr=&DDRB, .in=&PINB, .port=&PORTB, .mask= 1<<PB5 },
    [MCU_IO_USB_UART1_RX1] = { .ddr=&DDRD, .in=&PIND, .port=&PORTD, .mask= 1<<PD0 },
    [MCU_IO_USB_UART1_TX1] = { .ddr=&DDRD, .in=&PIND, .port=&PORTD, .mask= 1<<PD1 },
    [MCU_IO_SHUTDOWN] = { .ddr=&DDRD, .in=&PIND, .port=&PORTD, .mask= 1<<PD2 },
    [MCU_IO_BMC44] = { .ddr=&DDRD, .in=&PIND, .port=&PORTD, .mask= 1<<PD3 },
    [MCU_IO_BMC27] = { .ddr=&DDRD, .in=&PIND, .port=&PORTD, .mask= 1<<PD4 },
    [MCU_IO_BMC13_PWM1] = { .ddr=&DDRD, .in=&PIND, .port=&PORTD, .mask= 1<<PD5 },
    [MCU_IO_BMC12_PWM0] = { .ddr=&DDRD, .in=&PIND, .port=&PORTD, .mask= 1<<PD6 },
    [MCU_IO_BMC22] = { .ddr=&DDRD, .in=&PIND, .port=&PORTD, .mask= 1<<PD7 },
    [MCU_IO_ID_SD] = { .ddr=&DDRE, .in=&PINE, .port=&PORTE, .mask= 1<<PE0 },
    [MCU_IO_ID_SC] = { .ddr=&DDRE, .in=&PINE, .port=&PORTE, .mask= 1<<PE1 }
};
#else
#   error this is for an mega328pb manager on a PCB board, see https://github.com/epccs/Gravimetric
#endif

// read value from IO input bit and return its bool value
static inline __attribute__((always_inline))
bool ioRead(MCU_IO_t io) 
{
    return (*ioMap[io].in & ioMap[io].mask);
}

// set or clear IO output
static inline __attribute__((always_inline))
void ioWrite(MCU_IO_t io, LOGIC_LEVEL_t level) {
    ATOMIC_BLOCK ( ATOMIC_RESTORESTATE )
    {
        if (level) // == LOGIC_LEVEL_HIGH 
        {
            *ioMap[io].port |= ioMap[io].mask;
        } 
        else 
        {
            *ioMap[io].port &= ~ioMap[io].mask;
        }
    }
}

// toggle io (yes I know about the magic trick)
static inline __attribute__((always_inline))
void ioToggle(MCU_IO_t io) {
    ioWrite(io, (LOGIC_LEVEL_t) !ioRead(io));
}

// set io direction (INPUT or OUTPUT).
static inline __attribute__((always_inline))
void ioDir(MCU_IO_t io, DIRECTION_t dir) 
{
    ATOMIC_BLOCK ( ATOMIC_RESTORESTATE )
    {
        if (dir) // == DIRECTION_OUTPUT 
        {
            *ioMap[io].ddr |= ioMap[io].mask;
        } 
        else 
        {
            *ioMap[io].ddr &= ~ioMap[io].mask;
        }
    }
}

#endif // IO_Enum_h
