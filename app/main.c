/*
 * Project: Puzzle_box
 * Author: Oliver Edman Chrantz 
 * 
 * 
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include "common.h"
#include "timer.h"
#include "morse.h"




int main(void) {
    
    DDRB |= (1 << DDB0); // PB0 som utgång
    timer_init();
    sei();               // Aktivera interrupts för timern

    while(1) {
        
        morse_code_puzzle();

    }
}