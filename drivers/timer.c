#include <avr/io.h>         
#include <avr/interrupt.h>  
#include "timer.h"         


static volatile uint32_t ms_ticks = 0;

void timer_init(void) {
    // CTC mode (Clear Timer on Compare Match)
    TCCR0A = (1 << WGM01);
    
    // Prescaler 64
    TCCR0B = (1 << CS01) | (1 << CS00);
    
    // 1ms vid 16MHz (16000000 / 64 / 1000 = 250 steg, 0-249)
    OCR0A = 249;
    
    // Enable interrupt för Compare Match A
    TIMSK0 |= (1 << OCIE0A);
}

// Körs varje millisekund
ISR(TIMER0_COMPA_vect) {
    ms_ticks++;
}

uint32_t time_ms(void) {
    uint32_t current_time;
    
    // Atomär läsning: stäng av interrupts temporärt så vi inte 
    // läser byten precis när den slår om
    cli();
    current_time = ms_ticks;
    sei();
    
    return current_time;
}