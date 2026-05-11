#ifndef MORSE_H_
#define MORSE_H_

/**
 * @file morse.h
 * @brief Morse signaling module using software timer callbacks.
 */

/**
 * @brief Initializes GPIOs and registers the timer for Morse code.
 */
void morse_init(void);

/**
 * @brief Starts the Morse sequence.
 */
void morse_start(void);

/**
 * @brief Stops the sequence and turns off LEDs.
 */
void morse_stop(void);

#endif
