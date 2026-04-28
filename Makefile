MCU = atmega328p
F_CPU = 16000000UL
CC = avr-gcc
OBJCOPY = avr-objcopy
CFLAGS = -Wall -Os -mmcu=$(MCU) -DF_CPU=$(F_CPU) -Iinclude

# HÄR LÄGGER VI TILL drivers/adc.c
SRC = app/main.c src/morse.c src/timer.c src/adc.c src/spi.c
OBJ = $(SRC:.c=.o)

all: main.hex

main.elf: $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o main.elf

main.hex: main.elf
	$(OBJCOPY) -j .text -j .data -O ihex main.elf main.hex

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

flash: main.hex
	avrdude -p $(MCU) -c arduino -P /dev/ttyACM0 -b 115200 -U flash:w:main.hex:i

clean:
	rm -f app/*.o src/*.o drivers/*.o *.elf *.hex

.PHONY: all clean flash