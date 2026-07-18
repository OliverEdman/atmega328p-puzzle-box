/**
 * @file distance.h
 * @brief Interface for the HC-SRF04 sensor using a static pool.
 * @author Oliver Edman <o.edman@icloud.com>
 *
 * @note Licensed under the MIT License.
 */


#pragma once

#include <stdint.h>
#include <stdbool.h>

struct ultrasonic;

void ultrasonic_init();

struct ultrasonic *ultrasonic_new(uint8_t trig_pin, uint8_t echo_pin);

void ultrasonic_delete(struct ultrasonic **self);

void ultrasonic_trigger(struct ultrasonic *self);

float ultrasonic_get_distance(const struct ultrasonic *self);

bool ultrasonic_is_ready(const struct ultrasonic *self);

void ultrasonic_set_timeout_us(struct ultrasonic *self, uint32_t timeout_us);







