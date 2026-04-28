#include "display.h"
#include "spi.h"
#include <avr/io.h>

/* * Look-up Table för 7-segmentsdisplay (Gemensam Katod)
 * Mappning baserad på din lista: Q0=A, Q1=B, Q2=C, Q3=D, Q4=E, Q5=F, Q6=G
 * Bit-ordning i byten: [bit7: (oanvänd/DP), bit6: G, bit5: F, bit4: E, bit3: D, bit2: C, bit1: B, bit0: A]
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

/* * Digit selection map (vilken siffra som ska lysa)
 * Om Chip 2 styr katoderna för de 4 siffrorna:
 */
static const uint8_t digit_map[] = {
    0x01, // Siffra 1 (Längst till vänster)
    0x02, // Siffra 2
    0x04, // Siffra 3
    0x08  // Siffra 4 (Längst till höger)
};

void display_init(void) {
    spi_init(); // Initiera SPI-hårdvaran
    display_clear();
}

void display_show_number(uint8_t number) {
    if (number > 9) return;

    // Vi hämtar mönstret för siffran
    uint8_t segments = segment_map[number];
    
    // Vi skickar till skiftregisterna
    // Eftersom de är seriekopplade "skjuts" den första byten till det bakre chippet
    spi_write(segments);      // Skickas till Chip 2 (Segmenten)
    spi_write(digit_map[0]);  // Skickas till Chip 1 (Position - vi kör siffra 1 som test)
    
    spi_latch(); // Aktivera utgångarna
}

void display_clear(void) {
    spi_write(0xFF); // släck alla segment
    spi_write(0x00); // Avaktivera alla siffror
    spi_latch();
}

void display_write_pos(uint8_t number, uint8_t pos) {
    if (number > 9 || pos > 3) return;

    uint8_t segments = segment_map[number];
    uint8_t digit = digit_map[pos];

    spi_write(segments); // Skickas till Chip 2 (Segment)
    spi_write(digit);    // Skickas till Chip 1 (Position)
    spi_latch();
}