/**
 * @file pot_to_digits.h
 * @author Oliver Edman <o.edman@icloud.com>
 * @brief Logic for pot to digits puzzle.
 */

#pragma once

#include <stdbool.h>

void pot_to_digits_init(void);

void pot_to_digits_start(void);

void pot_to_digits_stop(void);

// Return True if puzzle is solved, false otherwise used for LED that indicate if puzzle is solved or not. 
bool pot_to_digits_is_solved(void);
