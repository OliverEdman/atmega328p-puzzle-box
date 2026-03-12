# --- Inställningar ---
MCU = atmega328p
F_CPU = 16000000UL
CC = avr-gcc
OBJCOPY = avr-objcopy

# Stränga inställningar: Alla varningar blir fel (-Werror)
CFLAGS = -Wall -Wextra -Werror -Os -mmcu=$(MCU) -DF_CPU=$(F_CPU) -Iinclude

# --- Filer ---
SRC = app/main.c src/morse.c drivers/timer.c
OBJ = $(SRC:.c=.o)

# --- Uppladdning (Flash) ---
PORT = /dev/ttyACM0
BAUD = 115200
PROGRAMMER = arduino

# --- Regler ---

all: main.hex

# Länka ihop
main.elf: $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o main.elf

# Skapa HEX
main.hex: main.elf
	$(OBJCOPY) -j .text -j .data -O ihex main.elf main.hex

# Kompilera .c till .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Ladda upp till USB
flash: main.hex
	avrdude -v -p $(MCU) -c $(PROGRAMMER) -P $(PORT) -b $(BAUD) -U flash:w:main.hex:i

# Städa
clean:
	rm -f app/*.o src/*.o drivers/*.o *.elf *.hex

.PHONY: all clean flash