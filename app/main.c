#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include "spi.h"
#include "timer.h"

const uint8_t SEGMENTS[] = {
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

const uint8_t DIGITS[] = {
    0xFE,
    0xFD,
    0xFB,
    0xF7
};

volatile uint8_t display_buffer[4] = {0x3F, 0x3F, 0x3F, 0x3F};

uint16_t counter = 0;

void display_multiplex_callback(void) {
    static uint8_t current_digit = 0;

    
    spi_write(DIGITS[current_digit]);
    spi_write(display_buffer[current_digit]);
    spi_latch();

    current_digit++;
    if (current_digit >= 4) {
        current_digit = 0;
    }
}

void counter_increment_callback(void) {
    counter++;
    if (counter > 9999) {
        counter = 0;
    }

    display_buffer[0] = SEGMENTS[(counter / 1000) % 10]; 
    display_buffer[1] = SEGMENTS[(counter / 100) % 10];
    display_buffer[2] = SEGMENTS[(counter / 10) % 10];
    display_buffer[3] = SEGMENTS[counter % 10];
}


int main(void) {

    spi_init();
    timer_init(); 

    struct timer *display_timer = timer_new(2, display_multiplex_callback);
    struct timer *counter_timer = timer_new(500, counter_increment_callback);

    timer_start(display_timer);
    timer_start(counter_timer);

    while (1) {
        
        timer_handler();

    }

    return 0;
}
