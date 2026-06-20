#define F_CPU 16000000UL
#include <avr/interrupt.h>
#include <stddef.h>
#include <stdbool.h>

#include "timer.h"
#include "gpio.h"
#include "morse.h"
#include "pot_to_digits.h"

int main(void) {

    timer_init(); 

    gpio_t* morse_led = gpio_new(6, GPIO_DIRECTION_OUTPUT, NULL);
    gpio_t* puzzle_solved_led = gpio_new(7, GPIO_DIRECTION_OUTPUT, NULL);

    pot_to_digits_init();
    pot_to_digits_start();

    morse_start(morse_led);

    sei(); 

    while (1) {

        timer_handler();

        
        if (pot_to_digits_is_solved()) {
            
            pot_to_digits_stop(); 
            
            gpio_write(puzzle_solved_led, true); 
        }
    }

    gpio_delete(&morse_led);
    gpio_delete(&puzzle_solved_led);
    return 0;
}
