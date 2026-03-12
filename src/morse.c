
#include "../include/morse.h"
#include <avr/io.h>
#include <stdbool.h>
#include "timer.h" 





/********************************************************
* Function name: morse_code_puzzle
* Parameter 1: 
* Parameter 2: 
* Return: Integer 
* Description:
* 
* 
********************************************************/


static uint32_t last_morse_change = 0;
static bool led_on = false;
static uint8_t index = 0; 

// 1 = kort, 2 = lång, 0 = paus
static uint8_t pattern[] = {1, 1, 1, 1, 1, 0, 2, 2, 2, 2, 2, 0, 0, 0};


#define PATTERN_LENGTH (sizeof(pattern) / sizeof(pattern[0]))

void morse_code_puzzle(void) {
    uint32_t current_time = time_ms();
    
    uint8_t current_value = pattern[index];
    uint32_t wait_time = 0;

    if (led_on == true) {
        if (current_value == 1) {
            wait_time = 200;  // Dot
        } else {
            wait_time = 600;  // Dash
        }
    } else {
        if (current_value == 0) {
            wait_time = 1000;
        } else {
            wait_time = 200;
        } 
    } 

    if (current_time - last_morse_change >= wait_time) {
        last_morse_change = current_time;

        if (led_on == true) {
            PORTB &= ~(1 << PORTB0); // Släck
            led_on = false;
            index++; 
        } else {
            if (current_value != 0) {
                PORTB |= (1 << PORTB0); // Tänd
                led_on = true;
            } else {
                index++;
            }
        }

        if (index >= PATTERN_LENGTH) {
            index = 0;
        }
    }
}