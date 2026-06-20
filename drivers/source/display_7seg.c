/**
 * @file display_7seg.c
 * @brief Driver implementation for 7-segment display using SPI and shift registers.
 */

#include "display_7seg.h"
#include "spi.h"
#include <avr/io.h>

/**
 * @brief Look-up Table for 7-segment display.
 */
static const uint8_t segment_map[] = {
    0x3F, // 0
    0x06, // 1
    0x5B, // 2
    0x4F, // 3
    0x66, // 4
    0x6D, // 5
    0x7D, // 6
    0x07, // 7
    0x7F, // 8
    0x6F  // 9
};

static const uint8_t digit_map[] = {
    0xFE, // 1
    0xFD, // 2
    0xFB, // 3
    0xF7 //  4
};

void display_init(void) {
    spi_init();
    display_clear();
}

void display_clear(void) {
    
    spi_write(0xFF); 
    spi_write(0x00);
    spi_latch();
}

void display_show_number(uint8_t number) {
    
    display_write_pos(number, 0);
}

void display_write_pos(uint8_t number, uint8_t pos) {
    if (number > 9 || pos > 3) return;

    spi_write(digit_map[pos]);

    spi_write(segment_map[number]);

    spi_latch();
}
