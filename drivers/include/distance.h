/**
 * @file distance.h
 * @brief Interface for an HC-SR04 ultrasonic distance sensor.
 * @author Oliver Edman <o.edman@icloud.com>
 *
 * @note Licensed under the MIT License.
 */

#pragma once

#include <stdint.h>
#include <stdbool.h>

/**
 * @brief Forward declaration of the opaque ultrasonic sensor struct.
 */
struct ultrasonic;

/**
 * @brief Initialize the hardware pins and timers for the ultrasonic sensor.
 *
 * Configures the trigger pin as output and the echo pin as input.
 */
void ultrasonic_init(void);

/**
 * @brief Create a new ultrasonic sensor instance.
 *
 * @param[in] trig_pin The GPIO pin connected to the Trig input.
 * @param[in] echo_pin The GPIO pin connected to the Echo output.
 * 
 * @return Pointer to the ultrasonic sensor instance, or NULL if fails.
 */
struct ultrasonic *ultrasonic_new(uint8_t trig_pin, uint8_t echo_pin);

/**
 * @brief Delete an ultrasonic sensor instance.
 *
 * @param[in,out] self Double pointer to the sensor instance to delete.
 */
void ultrasonic_delete(struct ultrasonic **self);

/**
 * @brief Trigger a distance measurement.
 *
 * This function sends the 10µs trigger pulse.
 *
 * @param[in] self Pointer to the ultrasonic sensor instance.
 */
void ultrasonic_trigger(struct ultrasonic *self);

/**
 * @brief Get the last measured distance in centimeters.
 *
 * @param[in] self Pointer to the ultrasonic sensor instance.
 * @return Distance in centimeters.
 */
float ultrasonic_get_distance_cm(const struct ultrasonic *self);

/**
 * @brief Check if the sensor has finished last measurement.
 *
 * @param[in] self Pointer to the ultrasonic sensor instance.
 * @return true if a new value is ready to be read, false otherwise.
 */
bool ultrasonic_is_ready(const struct ultrasonic *self);

/**
 * @brief Set a timeout limit for the echo pulse.
 *
 * Prevents the system from waiting if the sensor fails to return an echo.
 *
 * @param[in] self Pointer to the ultrasonic sensor instance.
 * @param[in] timeout_us Timeout in microseconds.
 */
void ultrasonic_set_timeout_us(struct ultrasonic *self, uint32_t timeout_us);
