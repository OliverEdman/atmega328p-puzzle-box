#include "spi.h"
#include <avr/io.h>

void spi_init(void) {
    /* * 1. Konfigurera DDRB (Data Direction Register B):
     * D11 (PB3/MOSI) -> SER på 595
     * D13 (PB5/SCK)  -> SRCLK på 595
     * D10 (PB2/SS)   -> RCLK (Latch) på 595
     */
    DDRB |= (1 << PB3) | (1 << PB5) | (1 << PB2);

    /* * 2. SPI Control Register (SPCR):
     * SPE  = Enable SPI
     * MSTR = Master mode (Arduinon styr klockan)
     * SPR0 = F_CPU / 16 = 1 MHz (Säker hastighet för 595 och breadboards)
     */
    SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0);
}

void spi_write(uint8_t data) {
    /* Skriv data till SPI Data Register - hårdvaran börjar skifta direkt */
    SPDR = data;

    /* Vänta på SPI Interrupt Flag (SPIF) i Status-registret (SPSR) 
     * Den blir 1 när alla 8 bitar har skickats.
     */
    while (!(SPSR & (1 << SPIF)));
}

void spi_latch(void) {
    /* * RCLK (Latch) triggas vid en "rising edge" (låg till hög).
     * Detta flyttar datan från skiftregistret till de faktiska utgångarna (Q0-Q7).
     */
    PORTB &= ~(1 << PB2); // Säkerställ låg
    // Ingen delay behövs vid 16MHz, hårdvaran hinner med
    PORTB |= (1 << PB2);  // Dra hög för att uppdatera displayen
}