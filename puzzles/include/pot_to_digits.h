/**
 * @file pot_to_digits.h
 * @author Oliver Edman <o.edman@icloud.com>
 * @brief Logic for pot to digits puzzle.
 */

#pragma once

#include <stdbool.h>

/**
 * @brief Initialize the potentiometer-to-digits puzzle.
 */
void pot_to_digits_init(void);

/**
 * @brief Start running the puzzle logic.
 */
void pot_to_digits_start(void);

/**
 * @brief Stop running the puzzle logic.
 */
void pot_to_digits_stop(void);

/**
 * @brief Check if the puzzle has been successfully solved.
 * @return true if the puzzle is solved, false otherwise.
 */
bool pot_to_digits_is_solved(void);
