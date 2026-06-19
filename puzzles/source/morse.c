/**
 * @file morse.c
 * @brief Implementation of the Morse code puzzle module.
 *This module controls an LED on digital pin 6 (PD6) (MORSE_LED)
 * Blinking morse number "55" using a non blocking software timer.
 */

#include "morse.h"
#include "gpio.h"
#include "timer.h"

#include <avr/io.h>
#include <stdbool.h>
#include <stddef.h>

#define DOT          200  /* Duration of a short blink (Dot) */
#define DASH         600  /* Duration of a long blink (Dash) */

#define SHORT_PAUSE  400  /* Off-time between elements within the same digit */
#define DIGIT_PAUSE  1200 /* Off-time between the two individual digits */
#define WORD_PAUSE   3000 /* Off-time at the end of the sequence before looping */

#define MORSE_STEPS 20

#define MORSE_LED PD6

static const uint16_t morse_sequence[MORSE_STEPS] = {
    // Morse code for the first number 5 
    DOT, SHORT_PAUSE, DOT, SHORT_PAUSE, DOT, SHORT_PAUSE, DOT, SHORT_PAUSE, DOT,
    
    // Pause between the first 5 and the second 5
    DIGIT_PAUSE,
    
    // Morse code for the second number 5 
    DOT, SHORT_PAUSE, DOT, SHORT_PAUSE, DOT, SHORT_PAUSE, DOT, SHORT_PAUSE, DOT,
    
    // Longer pause before the entire sequence loops back from the start
    WORD_PAUSE
};

/* Tracks the current active index in the morse_sequence array. */
static uint8_t current_step = 0;

/* Handle to the software timer instance assigned to this module. */
static struct timer* morse_timer = NULL;

static gpio_t* morse_led_pin = NULL;

/**
 * @brief Internal callback function triggered on every timer expiration.
 * Advances through the Morse sequence, toggles the LED state on PD6 based 
 * on whether the current step index is even (ON) or odd (OFF), and reloads 
 * the timer with the duration of the current step.
 */
static void morse_callback(void) {
    // Fetch the duration for the CURRENT step
    uint16_t duration = morse_sequence[current_step];

    // Control the LED based on the step index
    // Even index (0, 2, 4...) turns ON the LED (DOT/DASH)
    // Odd index (1, 3, 5...) turns OFF the LED (PAUSE)
    if (current_step % 2 == 0) {
        gpio_write (morse_led_pin, true);  // Set digital pin 6 to HIGH
    } else {
        gpio_write (morse_led_pin, false); // Set digital pin 6 to LOW
    }

    // Inform the timer how long to wait for this specific step
    timer_set_timeout_ms(morse_timer, duration);
    
    // Move forward to the next step
    current_step++;

    // If the sequence is finished, reset the index for the NEXT timer expiration
    if (current_step >= MORSE_STEPS) {
        current_step = 0; 
    }
}


/**
 * @brief Starts the Morse code puzzle asynchronously.
 */
void morse_start(gpio_t* t) {

    current_step = 0;

    morse_led_pin = t;

    if (morse_timer == NULL) {
        // Create a new timer with the first duration and point it to the callback
        morse_timer = timer_new(morse_sequence[0], morse_callback);
    } else {
        // If the timer already exists, force it to reload the first step duration
        timer_set_timeout_ms(morse_timer, morse_sequence[0]);
    }
    
    if (morse_timer != NULL) {
        timer_start(morse_timer);
    }
}

/**
 * @brief Stops the Morse code puzzle completely and resets the step index.
 */
void morse_stop(void) {
    if (morse_timer != NULL) {
        timer_stop(morse_timer);
    }
    gpio_write(morse_led_pin, false); // Ensure Morse LED is turned OFF
    current_step = 0;
}
