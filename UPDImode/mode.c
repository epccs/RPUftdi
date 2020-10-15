/*
bcm24 is a library that can pull-up or pull-down a pin connected to where the R-Pi BCM24 pin is located.
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
*/
#include <stdio.h>
#include <stdlib.h> 
#include <avr/pgmspace.h>
#include "../lib/parse.h"
#include "../lib/io_enum_bsd.h"
#include "../i2c-debug/id.h"
#include "mode.h"

void UPDI_mode(void)
{
    if (command_done == 10)
    {
        ioWrite(MCU_IO_BCM23,LOGIC_LEVEL_LOW); // not in UART mode
        ioWrite(MCU_IO_BCM24,LOGIC_LEVEL_HIGH); // in UPDI mode
        printf_P(PSTR("{\"PiUPDI\":\"UPDI\"}\r\n"));
        initCommandBuffer();
    }

    else
    {
        initCommandBuffer();
    }
}

void UART_mode(void)
{
    if (command_done == 10)
    {
        ioWrite(MCU_IO_BCM23,LOGIC_LEVEL_HIGH); // in UART mode
        ioWrite(MCU_IO_BCM24,LOGIC_LEVEL_LOW); // not in UPDI mode
        printf_P(PSTR("{\"PiUPDI\":\"UART\"}\r\n"));
        initCommandBuffer();
    }

    else
    {
        initCommandBuffer();
    }
}