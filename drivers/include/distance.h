/**
 * @file distance.h 
 * @brief Interface for the HY-SRF05 Ultrasonic sensor driver module.
 * @author Oliver Edman <o.edman@icloud.com>
 *
 * @note Licensed under the MIT License.
 */

#pragma once

#include <stdint.h>


/**
 * @brief Initialize the ultrasonic sensor pins.
 * Configures Echo (PD2) as input.
 */
void ultrasonic_init();

/**
 * @brief Triggers a distance measurement.
 * NOTE: Since Trigger is on Reg3 (Q7), this function needs to know 
 * the current 8-bit frame of Reg3 to set Q7 high for 10 microseconds.
 *
 * @param current_reg3_state Current byte representation of Reg3 outputs.
 *
 * @return uint16_t Measured distance in centimeters (cm). Returns 0 if out of range/timeout.
 */
uint16_t ultrasonic_read_distance (uint8_t current_reg3_state);
