#pragma once

/**
 * @file morse.h
 * @brief Morse code for ATmega328P.
 * * This module provides functions to initialize and control a Morse code 
 */

#include <stdint.h>

/**
 * @brief Initializes the Morse code.
 * * Configures the necessary GPIO pins as outputs and ensures the 
 * Morse LED is (OFF).
 */
void morse_init(void);

/**
 * @brief Starts the Morse code.
 * * Executes the Morse sequence.
 * This function handles the timing for dots, dashes, and pauses.
 * 
 */
void morse_start(void);

/**
 * @brief Must be called repeatedly in the main loop. 
 */
void morse_tick(void);

/**
 * @brief Immediately stops the Morse code transmission.
 * * Forces the signaling LED to LOW.
 */
void morse_stop(void);