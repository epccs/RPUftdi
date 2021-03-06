# build application from source
# when files for application change the binary image needs updated
# https://www.gnu.org/software/make/manual/make.html
TARGET = blinkLED
LIBDIR = ../lib
OBJECTS = main.o \
	mode.o \
	../i2c-debug/id.o \
	$(LIBDIR)/twi1_bsd.o \
	$(LIBDIR)/uart0_bsd.o \
	$(LIBDIR)/timers_bsd.o \
	$(LIBDIR)/parse.o

# Chip and project-specific global definitions
# VSCode note: avr-gcc will look for device-specs/specs-atmega328pb and use that to define __AVR_DEV_LIB_NAME__ = m328pb
MCU = atmega328pb
F_CPU = 12000000UL
CPPFLAGS = -DF_CPU=$(F_CPU) -I.

LOCK_FUSE = 0xEF

## Cross-compilation
CC = avr-gcc
OBJCOPY = avr-objcopy
OBJDUMP = avr-objdump
SIZE = avr-size

# FTDI's USB to serial bridge shows as /dev/ttyUSB0, 
# Uno's serial bridge (an ATmega16U2) shows as /dev/ttyACM0  (a modem,?)
# Pi Zero on chip hardware serial shows as /dev/ttyAMA0 (hardware UART on a Linux system)
detect_PORT := $(shell sh -c 'ls /dev/ttyUSB1 2>/dev/null || echo not')
ifeq ($(detect_PORT),/dev/ttyUSB1)
BOOTLOAD_PORT = /dev/ttyUSB1
else 
BOOTLOAD_PORT =
endif

## Compiler/linker options
CFLAGS = -Os -g -std=gnu99 -Wall
# CFLAGS += -funsigned-char -funsigned-bitfields 
# CFLAGS += -fpack-struct -fshort-enums 
CFLAGS += -ffunction-sections -fdata-sections

TARGET_ARCH = -mmcu=$(MCU) -B $(LIBDIR)/ATmega_DFP/gcc/dev/atmega328pb/ -I $(LIBDIR)/ATmega_DFP/include/
## if atmega324pb is in avr-gcc mainline use
##TARGET_ARCH = -mmcu=$(MCU)

LDFLAGS = -Wl,-Map,$(TARGET).map
LDFLAGS += -Wl,--gc-sections

.PHONY: help

# some help for the make impaired
# https://marmelab.com/blog/2016/02/29/auto-documented-makefile.html
help:
	@grep -E '^[a-zA-Z_-]+:.*?## .*$$' $(MAKEFILE_LIST) | sort | awk 'BEGIN {FS = ":.*?## "}; {printf "\033[36m%-30s\033[0m %s\n", $$1, $$2}'

all: $(TARGET).hex $(TARGET).lst ## build the image and its related files

$(TARGET): $(TARGET).hex

$(TARGET).hex: $(TARGET).elf
	$(OBJCOPY) -j .text -j .data -O ihex $< $@

# optiboot erases flash without being told (e.g. -e )
bootload: ## upload to optiboot
ifndef BOOTLOAD_PORT 
	$(error error no bootload port found $(BOOTLOAD_PORT))
endif
	avrdude -v -p $(MCU) -C +$(LIBDIR)/avrdude/328pb.conf -c arduino -P $(BOOTLOAD_PORT) -b 38400 -U flash:w:$(TARGET).hex

hdrcode: ## copy header for VSCode to find run with sudo
	@echo VSCode will not look outside or cross-origin for an include,  
	@echo so I have to put the MCU header where it can find it.
	cp -u ../lib/ATmega_DFP/include/avr/iom328pb.h /usr/lib/avr/include/avr/iom328pb.h
	chmod u-x /usr/lib/avr/include/avr/iom328pb.h

$(TARGET).elf: $(OBJECTS)
	$(CC) $(LDFLAGS) $(TARGET_ARCH) $^ -o $@
	@echo binutils-avr do not have 328pb, but it is sized like a 328p
	$(SIZE) -C --mcu=atmega328p $@
	rm -f $(TARGET).o $(OBJECTS)

clean: ## remove the image and its related files
	rm -f $(TARGET).hex $(TARGET).map $(TARGET).elf $(TARGET).lst
 
%.lst: %.elf
	$(OBJDUMP) -h -S $< > $@
