/**
 * @file display.c
 * @brief Driver implementation for 7-segment display using SPI and shift registers.
 */

#include "display_7seg.h"
#include "spi.h"
#include <avr/io.h>

/**
 * @brief Look-up Table for 7-segment display (Common Cathode/Anode dependent).
 * * Mapping based on: Q0=A, Q1=B, Q2=C, Q3=D, Q4=E, Q5=F, Q6=G.
 */
static const uint8_t segment_map[] = {
    0x3F, // 0: 0011 1111
    0x06, // 1: 0000 0110
    0x5B, // 2: 0101 1011
    0x4F, // 3: 0100 1111
    0x66, // 4: 0110 0110
    0x6D, // 5: 0110 1101
    0x7D, // 6: 0111 1101
    0x07, // 7: 0000 0111
    0x7F, // 8: 0111 1111
    0x6F  // 9: 0110 1111
};

/**
 * @brief Digit selection map to activate specific display positions.
 * * Defines which digit (1-4) receives power or ground via the first shift register.
 */
static const uint8_t digit_map[] = {
    0x01, // Siffra 1 (left)
    0x02, // Siffra 2
    0x04, // Siffra 3
    0x08  // Siffra 4 (right)
};

/**
 * @brief Initializes the display system.
 * * Sets up SPI hardware and ensures the display is cleared at startup.
 */

void display_init(void) {
    spi_init(); // Initiera SPI-hårdvaran
    display_clear();
}

/**
 * @brief Displays a single digit on the first position.
 * @param number The digit to display (0-9).
 */

void display_show_number(uint8_t number) {
    if (number > 9) return;

    // Vi hämtar mönstret för siffran
    uint8_t segments = segment_map[number];
    
    // send to shift registers
    spi_write(segments);      // Skickas till Chip 2 
    spi_write(digit_map[0]);  // Skickas till Chip 1 
    
    spi_latch(); // outputs
}

/**
 * @brief Clears the entire display.
 * * Turns off all segments and deactivates all digit positions.
 */

void display_clear(void) {
    spi_write(0xFF); // turn off all segments
    spi_write(0x00); // deactive all numbers
    spi_latch();
}

/**
 * @brief Writes a specific number to a specific display position.
 * * Used for multiplexing multiple digits across the 4-digit display.
 * @param number The digit to display (0-9).
 * @param pos The position to use (0=Leftmost, 3=Rightmost).
 */

void display_write_pos(uint8_t number, uint8_t pos) {
    if (number > 9 || pos > 3) return;

    uint8_t segments = segment_map[number];
    uint8_t digit = digit_map[pos];

    spi_write(segments); // Chip 2 (Segment)
    spi_write(digit);    // Chip 1 (Position)
    spi_latch();
}
