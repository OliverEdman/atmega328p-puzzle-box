cmake_minimum_required(VERSION 3.15)

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR avr)
set(CMAKE_C_COMPILER avr-gcc)
set(CMAKE_OBJCOPY avr-objcopy)

project(PuzzleBox C)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(MCU "atmega328p")
set(F_CPU "16000000UL")
set(CMAKE_C_FLAGS "-Wall -Os -mmcu=${MCU} -DF_CPU=${F_CPU}")

include_directories(
    drivers/include
    puzzles/include
    lib
)

set(SRC_FILES
    app/main.c
    drivers/source/timer.c
    drivers/source/adc.c
    drivers/source/spi.c
    drivers/source/gpio.c
    drivers/source/display_7seg.c
    drivers/source/uart.c
    puzzles/source/morse.c
    puzzles/source/pot_to_digits.c
    
)

add_executable(main.elf ${SRC_FILES})

add_custom_command(
    OUTPUT main.hex
    COMMAND ${CMAKE_OBJCOPY} -j .text -j .data -O ihex main.elf main.hex
    DEPENDS main.elf
    COMMENT "Generating main.hex from main.elf"
)

add_custom_target(flash
    COMMAND avrdude -p atmega328p -c arduino -P /dev/ttyACM0 -b 115200 -U flash:w:main.hex:i
    DEPENDS main.hex
    COMMENT "Flashing main.hex to ATmega328P..."
)
