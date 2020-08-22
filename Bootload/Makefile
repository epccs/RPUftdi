# build application from source
# when files for application change the binary image needs updated
# https://www.gnu.org/software/make/manual/make.html
TARGET = optiboot_rpuusb
LIBDIR = ../lib
OBJECTS = main.o

# Chip and project-specific global definitions
MCU = atmega328pb
F_CPU = 12000000UL
BAUD = 38400UL
LED = B5
LED_START_FLASHES = 0
BOOT_FLASH_SECTION_START = 0x7e00
VERSION_PLACED_AT_FLASH_LOCATION = 0x7ffe

# http://eleccelerator.com/fusecalc/fusecalc.php?chip=atmega328p&LOW=FF&HIGH=DE&EXTENDED=05&LOCKBIT=3F
# VCC is 3V3 so BOD needs to be set at 2V7
# the 328pb has an extra efuse bit (3) that should not be set (e.g., 1) but it has been seen from digi-key with a set value (e.g., 0).
LOW_FUSE = 0xFF
HIGH_FUSE = 0xDE
EXTENDED_FUSE = 0xFD
UNLOCK_FUSE = 0xFF
LOCK_FUSE = 0xEF

# note BAUD is used as register on new AVR (m4809, AVR128DA)
CPPFLAGS = -DF_CPU=$(F_CPU) -DBAUD=$(BAUD)
CPPFLAGS += -DLED_START_FLASHES=$(LED_START_FLASHES)
CPPFLAGS += $(LED_DATA_FLASH_CMD) -DLED=$(LED) 

# BIGBOOT adds extra features, but takes up 1K.
# CPPFLAGS += -DBIGBOOT=1

# SOFT_UART: AVR305 soft-UART instead of hardware UART
# CPPFLAGS += -DSOFT_UART=1

# LED_DATA_FLASH: Flash LED when transferring data
# CPPFLAGS += -DLED_DATA_FLASH=1

# UART: number (0..n) for devices with more than  one hardware uart (644P, 1284P, etc)
# CPPFLAGS += -DUART=0

# Cross-compilation
CC = avr-gcc
OBJCOPY = avr-objcopy
OBJDUMP = avr-objdump
SIZE = avr-size

# /dev/ttyUSB0 is for FTDI's USB to serial bridge which I do not use to load a bootloader 
# /dev/ttyACM0 is (a modem?) for the ATmega16U2 on an Uno used as a serial bridge which I do use to load a bootloader    
# /dev/ttyAMA0 is a hardware UART on Linux, a Pi Zero has one good hardware serial but I don't want to spoil your fishing trip
ISP_PORT = /dev/ttyACM0

## set the environment variable used for compiler and linker options for a baudcheck to be done
# https://wiki.gentoo.org/wiki/GCC_optimization
CFLAGS  = -g -Wall -Os -fno-split-wide-types -mrelax -fno-caller-saves 

TARGET_ARCH = -mmcu=$(MCU) -B $(LIBDIR)/ATmega_DFP/gcc/dev/atmega328pb/ -I $(LIBDIR)/ATmega_DFP/include/
## if atmega328pb is in avr-gcc mainline use
##TARGET_ARCH = -mmcu=$(MCU)

# bootloader flags, e.g. omit the interrupt vector table and optiboot does not use a stack frame
# https://gcc.gnu.org/onlinedocs/gcc/Link-Options.html
# https://gcc.gnu.org/onlinedocs/gcc/AVR-Options.html#AVR-Options
LDFLAGS = -Wl,--section-start=.text=$(BOOT_FLASH_SECTION_START) -Wl,--section-start=.version=$(VERSION_PLACED_AT_FLASH_LOCATION) -Wl,--relax -nostartfiles -nostdlib
#-Wl,--gc-sections
# tbd: does the new option "-flto" have advantage over such things as "-ffunction-sections -fdata-sections" with "-Wl,--gc-sections -Wl,--print-gc-sections"
# issue with stack on some compiler versions: https://github.com/Optiboot/optiboot/issues/160

.PHONY: help

# some help for the make impaired
# https://marmelab.com/blog/2016/02/29/auto-documented-makefile.html
help:
	@grep -E '^[a-zA-Z_-]+:.*?## .*$$' $(MAKEFILE_LIST) | sort | awk 'BEGIN {FS = ":.*?## "}; {printf "\033[36m%-30s\033[0m %s\n", $$1, $$2}'

all: $(TARGET)_$(MCU)_-b$(BAUD)_F_CPU$(F_CPU).hex $(TARGET)_$(MCU)_-b$(BAUD)_F_CPU$(F_CPU).lst ## build the image and its related files

# use the C preprocessor to make a shell script which bash can run to check the baudrate
baudcheck: 
	- @$(CC) --version
	- @$(CC) $(CFLAGS) -DF_CPU=$(F_CPU) -DBAUD=$(BAUD) -E baudcheck.c -o baudcheck.tmp.sh
	- @bash baudcheck.tmp.sh
	rm -f baudcheck.tmp.sh

# platforms support EEPROM and large bootloaders need the eeprom functions that
# are defined in libc (e.g. the -lc option), even though they were explicity remove with 
# -nostdlib to save space.
%.elf: $(OBJECTS) baudcheck
	$(CC) $(LDFLAGS) $(TARGET_ARCH) -o $@ $< -lc
	$(SIZE) -C --mcu=$(MCU) $@

fuse: ## use ISCP to set the fuse
	avrdude -v -p $(MCU) -C +$(LIBDIR)/avrdude/328pb.conf -c stk500v1 -P $(ISP_PORT) -b 19200 -e -U lock:w:$(UNLOCK_FUSE):m -U lfuse:w:$(LOW_FUSE):m -U hfuse:w:$(HIGH_FUSE):m -U efuse:w:$(EXTENDED_FUSE):m

# set fuse like they are from factory, i.e. 8MHz internal, B.O.D. disabled.
# http://eleccelerator.com/fusecalc/fusecalc.php?chip=atmega328p&LOW=62&HIGH=D9&EXTENDED=FF&LOCKBIT=FF
oem: ## use ISCP to set the fuse like from factory
	avrdude -v -p $(MCU) -C +$(LIBDIR)/avrdude/328pb.conf -c stk500v1 -P /dev/ttyACM0 -b 19200 -e -U lock:w:$(UNLOCK_FUSE):m -U lfuse:w:0x62:m -U hfuse:w:0xD9:m -U efuse:w:0xFF:m

# 328pb can lock the bootloader section which I may use at some point
isp: ## with Arduino as ISP sketch read the eeprom, set fuse, upload program image to flash
	avrdude -v -p $(MCU) -C +$(LIBDIR)/avrdude/328pb.conf -c stk500v1 -P $(ISP_PORT) -b 19200 -U eeprom:r:$(TARGET)_$(MCU)_eeprom.hex:i
	avrdude -v -p $(MCU) -C +$(LIBDIR)/avrdude/328pb.conf -c stk500v1 -P $(ISP_PORT) -b 19200 -e -U lock:w:$(UNLOCK_FUSE):m -U lfuse:w:$(LOW_FUSE):m -U hfuse:w:$(HIGH_FUSE):m -U efuse:w:$(EXTENDED_FUSE):m
	avrdude -v -p $(MCU) -C +$(LIBDIR)/avrdude/328pb.conf -c stk500v1 -P $(ISP_PORT) -b 19200 -e -U flash:w:$(TARGET)_$(MCU)_-b$(BAUD)_F_CPU$(F_CPU).hex -U lock:w:$(LOCK_FUSE):m

# The avrdude in raspian is from
# https://github.com/facchinm/avrdude
linuxspi: ## with an R-Pi SPI port read the eeprom, set fuse, upload program image to flash
	sudo avrdude -v -p $(MCU) -C +$(LIBDIR)/avrdude/328pb.conf -c linuxspi -P /dev/spidev0.0 -U eeprom:r:$(TARGET)_$(MCU)_eeprom.hex:i
	sudo avrdude -v -p $(MCU) -C +$(LIBDIR)/avrdude/328pb.conf -c linuxspi -P /dev/spidev0.0 -e -U lock:w:$(UNLOCK_FUSE):m -U lfuse:w:$(LOW_FUSE):m -U hfuse:w:$(HIGH_FUSE):m -U efuse:w:$(EXTENDED_FUSE):m
	sudo avrdude -v -p $(MCU) -C +$(LIBDIR)/avrdude/328pb.conf -c linuxspi -P /dev/spidev0.0 -e -U flash:w:$(TARGET)_$(MCU)_-b$(BAUD)_F_CPU$(F_CPU).hex -U lock:w:$(LOCK_FUSE):m

# recover a saved eeprom
recoveree: ## with Arduino as ISP sketch recover the saved eeprom
	avrdude -v -p $(MCU) -c stk500v1 -P $(ISP_PORT) -b 19200 -U eeprom:w:$(TARGET)_$(MCU)_eeprom.hex
	rm $(TARGET)_$(MCU)_eeprom.hex
	
ree_linuxspi: ## with an R-Pi SPI port recover the saved eeprom
	sudo avrdude -v -p $(MCU) -c linuxspi -P /dev/spidev0.0 -U eeprom:w:$(TARGET)_$(MCU)_eeprom.hex
	rm $(TARGET)_$(MCU)_eeprom.hex

clean: ## remove the image and its related files
	rm -f *.o *.elf *.lst *.map *.sym *.lss *.eep *.srec *.bin *.hex *.tmp.sh

%.lst: %.elf
	$(OBJDUMP) -h -S $< > $@

%.hex: %.elf
	$(OBJCOPY) -j .text -j .data -j .version --set-section-flags .version=alloc,load -O ihex $< $@

%.srec: %.elf
	$(OBJCOPY) -j .text -j .data -j .version --set-section-flags .version=alloc,load -O srec $< $@

%.bin: %.elf
	$(OBJCOPY) -j .text -j .data -j .version --set-section-flags .version=alloc,load -O binary $< $@
