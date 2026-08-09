/**
 * @file simon_says.h
 * @brief Interface for the simon says puzzle module.
 * @author Oliver Edman <o.edman@icloud.com>
 * @note Licensed under the MIT License.
 */

#pragma once

#include <stdbool.h>

/**
 * @brief Initializes the Simon Says puzzle module.
 */
void simon_says_init();

/**
 * @brief Start Simon Says puzzle.
 */
void simon_says_start();

/**
 * @brief Stops the Simon Says puzzle.
 * 
 * Aborts the current game session and turns off all LEDs.
 */
void simon_says_stop();

/**
 * @brief Checks if the Simon Says puzzle has been successfully solved.
 * 
 * @return true If the user has completed all 7 levels.
 * @return false If the puzzle is still active, failed, or not started.
 */
bool simon_says_solved();
