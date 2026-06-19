cmake_minimum_required(VERSION 3.15)

# 1. Tvinga CMake att använda AVR-kompilatorn
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR avr)
set(CMAKE_C_COMPILER avr-gcc)
set(CMAKE_OBJCOPY avr-objcopy)

# 2. Definiera projektet
project(PuzzleBox C)

# Tvinga fram generering av compile_commands.json för Neovim/clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# 3. Sätt MCU-inställningar och kompileringsflaggor
set(MCU "atmega328p")
set(F_CPU "16000000UL")
set(CMAKE_C_FLAGS "-Wall -Os -mmcu=${MCU} -DF_CPU=${F_CPU}")

# 4. Berätta var ALLA dina header-filer ligger
include_directories(
    drivers/include
    puzzles/include
    lib
)

# 5. Lista alla dina källkodsfiler med deras exakta sökvägar
set(SRC_FILES
    app/main.c
    puzzles/source/morse.c
    drivers/source/timer.c
    drivers/source/adc.c
    drivers/source/spi.c
)

# 6. Skapa den körbara binären (.elf)
add_executable(main.elf ${SRC_FILES})

# 7. Automatisera skapandet av .hex-filen efter att .elf har byggts
add_custom_command(
    TARGET main.elf POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -j .text -j .data -O ihex main.elf main.hex
    COMMENT "Generating main.hex from main.elf"
)

# 8. Lägg till flash-target för avrdude
add_custom_target(flash
    COMMAND avrdude -p atmega328p -c arduino -P /dev/ttyACM0 -b 115200 -U flash:w:main.hex:i
    DEPENDS main.hex
    COMMENT "Flashing main.hex to ATmega328P..."
)
