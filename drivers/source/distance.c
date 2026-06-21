#include "distance.h"
#include "spi.h"
#include "utils.h"
#include <avr/io.h>
#include <avr/interrupt.h>

static const uint8_t echo_pin = PD2;
static const uint8_t trigger_bit = 7;
static const uint16_t trigger_delay_ticks = 1;
static const uint32_t echo_timeout_loops = 60000;
static const uint32_t max_pulse_duration_us = 25000;
static const uint32_t ticks_to_us_multiplier = 1000;
static const uint8_t us_to_cm_divisor = 58;

extern volatile uint32_t system_ticks;

void ultrasonic_init(void) {
    CLEAR(DDRD, echo_pin);
}

uint16_t ultrasonic_read_distance(uint8_t current_reg3_state) {
    uint8_t reg3_trigger_high = current_reg3_state;
    SET(reg3_trigger_high, trigger_bit);

    uint8_t reg3_trigger_low = current_reg3_state;
    CLEAR(reg3_trigger_low, trigger_bit);

    spi_write(reg3_trigger_high); 
    spi_write(0x00);
    spi_write(0x00);
    spi_latch();

    uint16_t start_ticks = system_ticks;
    while ((uint16_t)(system_ticks - start_ticks) < trigger_delay_ticks);

    spi_write(reg3_trigger_low);
    spi_write(0x00);
    spi_write(0x00);
    spi_latch();

    uint32_t timeout = echo_timeout_loops;
    while (!READ(PIND, echo_pin)) {
        if (--timeout == 0) return 0; 
    }

    uint32_t pulse_duration = 0;
    while (READ(PIND, echo_pin)) {
        uint16_t loop_ticks = system_ticks;
        while ((uint16_t)(system_ticks - loop_ticks) < trigger_delay_ticks);
        pulse_duration += ticks_to_us_multiplier;
        
        if (pulse_duration > max_pulse_duration_us) break; 
    }

    uint16_t distance_cm = pulse_duration / us_to_cm_divisor;
    return distance_cm;
}
