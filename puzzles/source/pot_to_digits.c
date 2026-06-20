#include "pot_to_digits.h"
#include "display_7seg.h"
#include "adc.h"
#include "timer.h"
#include <stddef.h>
#include <stdlib.h> 

#define NOISE_THRESHOLD   3 
#define DISPLAY_MAX_VALUE 99

// Calibrations for the two physical potentiometer
#define POT1_CHANNEL      0    // Connected to A0 (PC0)
#define POT1_MAX_RAW      868  // Calibrated max for Pot 1

#define POT2_CHANNEL      1    // Connected to A1 (PC1)
#define POT2_MAX_RAW      868  // Calibrated max for Pot 2

// Target values for the secret combination
#define POT1_TARGET       55
#define POT2_TARGET       96

#define DP_MASK           0x80 

static volatile uint8_t current_digits[4] = {0, 0, 0, 0};
static volatile bool current_dps[4] = {false, false, false, false};
static bool is_puzzle_solved = false;
static struct timer* display_timer = NULL;
static struct timer* pot_timer = NULL;

/**
 * @brief Helper function to read, filter, and map a specific potentiometer channel.
 */
static uint16_t read_pot_channel(uint8_t channel, uint16_t max_raw, uint16_t* last_stable_adc) {
    uint16_t raw_adc = adc_read_raw(channel);
    uint16_t upper_trigger_limit = max_raw - 8;

    if (abs((int16_t)raw_adc - (int16_t)(*last_stable_adc)) > NOISE_THRESHOLD || 
        raw_adc == 0 || raw_adc > upper_trigger_limit) {
        
        *last_stable_adc = raw_adc;
    }

    if (*last_stable_adc > max_raw) {
        *last_stable_adc = max_raw;
    }

    return (uint16_t)(((uint32_t)(*last_stable_adc) * DISPLAY_MAX_VALUE) / max_raw);
}

/**
 * @brief Multiplexes the 4-digit display. Runs at a high frequency (e.g., every 2ms).
 */
static void display_multiplex_callback(void) {
    static uint8_t current_digit_idx = 0;
    
    uint8_t val = current_digits[current_digit_idx];
    
    if (current_dps[current_digit_idx]) {
        val |= DP_MASK; 
    }
    
    display_write_pos(val, current_digit_idx);
    
    current_digit_idx = (current_digit_idx + 1) % 4;
}

/**
 * @brief Reads both potentiometers and updates the respective display digits and DPs.
 */
static void pot_read_callback(void) {
    static uint16_t last_stable_pot1 = 0;
    static uint16_t last_stable_pot2 = 0;

    uint16_t pot1_mapped = read_pot_channel(POT1_CHANNEL, POT1_MAX_RAW, &last_stable_pot1);
    uint16_t pot2_mapped = read_pot_channel(POT2_CHANNEL, POT2_MAX_RAW, &last_stable_pot2);

    current_digits[0] = (pot1_mapped / 10) % 10;
    current_digits[1] = pot1_mapped % 10;
    current_digits[2] = (pot2_mapped / 10) % 10;
    current_digits[3] = pot2_mapped % 10;

    if (pot1_mapped == POT1_TARGET) {
        current_dps[0] = true;
        current_dps[1] = true;
    } else {
        current_dps[0] = false;
        current_dps[1] = false;
    }

    if (pot2_mapped == POT2_TARGET) {
        current_dps[2] = true;
        current_dps[3] = true;
    } else {
        current_dps[2] = false;
        current_dps[3] = false;
    }

    if (pot1_mapped == POT1_TARGET && pot2_mapped == POT2_TARGET) {
        is_puzzle_solved = true;
    } else {
        is_puzzle_solved = false;
    }
}

void pot_to_digits_init(void) {
    display_init(); 
    adc_init();

    display_timer = timer_new(2, display_multiplex_callback);
    pot_timer = timer_new(50, pot_read_callback);
}

void pot_to_digits_start(void) {
    timer_start(display_timer);
    timer_start(pot_timer);
    is_puzzle_solved = false;
}

void pot_to_digits_stop(void) {
    timer_stop(display_timer);
    timer_stop(pot_timer);
}

bool pot_to_digits_is_solved(void) {
    return is_puzzle_solved;
}
