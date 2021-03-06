# Name of the target (files will be named like that)
TARGET=avr-motor-driver

# Name of the Microcontroller
MCU=atmega328p

# Clock of the Microcontroller (Hz)
F_CPU=16000000

# Fuses
L_FUSE=0xFF
H_FUSE=0xDA
E_FUSE=0xFD

# Target where the files should be placed
BUILDPATH=./build

# List of all sources (only source-files)
SOURCES  = main.c
SOURCES += led.c
SOURCES += uart.c
SOURCES += ssd1306.c
SOURCES += i2cmaster.c
SOURCES += motor.c
SOURCES += encoder.c
SOURCES += menue.c
SOURCES += timer2.c
SOURCES += settings.c

# Progammer
PROGRAMMER=arduino
PORT=-P/dev/ttyUSB0
#BAUD=-B9600
BAUD=-b 57600

OBJECTS=$(SOURCES:%.c=$(BUILDPATH)/%.o)
	CFLAGS  = -c
	CFLAGS += -Os
	CFLAGS += -Wall
	CFLAGS += -fpack-struct
	CFLAGS += -fshort-enums
	CFLAGS += -ffunction-sections
	CFLAGS += -fdata-sections
	CFLAGS += -std=gnu99
	CFLAGS += -funsigned-char
	CFLAGS += -funsigned-bitfields
	CFLAGS += -DF_CPU=$(F_CPU)UL
	LDFLAGS=



all: clean hex eeprom flash_hex

list_targets:
	avr-gcc --target-help

hex: $(TARGET).hex size

eeprom: $(TARGET)_eeprom.hex

$(TARGET).hex: $(TARGET).elf
	avr-objcopy -O ihex -j .data -j .text $(BUILDPATH)/$(TARGET).elf $(BUILDPATH)/$(TARGET).hex

$(TARGET)_eeprom.hex: $(TARGET).elf
	avr-objcopy -O ihex -j .eeprom --change-section-lma .eeprom=1 $(BUILDPATH)/$(TARGET).elf $(BUILDPATH)/$(TARGET)_eeprom.hex

$(TARGET).elf: $(OBJECTS)
	avr-gcc $(LDFLAGS) -mmcu=$(MCU) $(OBJECTS) -o $(BUILDPATH)/$(TARGET).elf


$(BUILDPATH)/%.o: %.c
	mkdir -p $(BUILDPATH)
	avr-gcc $(CFLAGS) -mmcu=$(MCU) $< -o $@


size:
	avr-size --mcu=$(MCU) -C $(BUILDPATH)/$(TARGET).elf

flash_hex:
	avrdude -p $(MCU) $(PORT) $(BAUD) -c $(PROGRAMMER) -U flash:w:$(BUILDPATH)/$(TARGET).hex:a
	#	micronucleus --run $(BUILDPATH)/$(TARGET).hex

read_fuses:
	avrdude -p $(MCU) $(PORT) $(BAUD) -c $(PROGRAMMER) -U lfuse:r:-:i

flash_fuses:
	avrdude -p $(MCU) $(PORT) $(BAUD) -c $(PROGRAMMER) -U lfuse:w:$(L_FUSE):m -U hfuse:w:$(H_FUSE):m -U efuse:w:$(E_FUSE):m

clean_tmp:
	rm -rf $(BUILDPATH)/*.o
	rm -rf $(BUILDPATH)/*.elf

clean:
	rm -rf $(BUILDPATH)/*.o
	rm -rf $(BUILDPATH)/*.elf
	rm -rf $(BUILDPATH)/*.hex
