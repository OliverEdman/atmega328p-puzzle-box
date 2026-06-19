#include <avr/io.h>
#include <avr/interrupt.h>
#include <stddef.h>

#include "gpio.h"
#include "timer.h"
#include "morse.h"

int main(void) {
    
    timer_init();
    
    gpio_t* morse_led = gpio_new(6, GPIO_DIRECTION_OUTPUT, NULL);
        
    morse_start(morse_led);

    sei(); 

    while (1) {
        
        timer_handler();

    }

    gpio_delete(&morse_led);
    return 0;
}
