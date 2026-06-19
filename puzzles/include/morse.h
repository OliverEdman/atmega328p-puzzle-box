/**
 * @file morse.h
 * @brief Interface for the morse code.
 * @author Oliver Edman <o.edman@icloud.com>
 *
 * @note Licensed under the MIT License.
 */
#include "gpio.h"
#pragma once

/**
 * @brief Initialize the GPIO pin for Morse LED and timer registers.
 */
void morse_init(void);

/**
 * @brief Starts the Morse sequence.
 */
void morse_start(gpio_t* led);

/**
 * @brief Stops the sequence and turns off LEDs.
 */
void morse_stop(void);

