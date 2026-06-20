#include "pot_to_digits.h"
#include "display_7seg.h"
#include "adc.h"
#include "timer.h"
#include <stddef.h>

static volatile uint8_t current_digits[4] = {0, 0, 0, 0};
static bool is_puzzle_solved = false;
static uint16_t counter = 0; 
static struct timer* display_timer = NULL;
static struct timer* counter_timer = NULL;

static void display_multiplex_callback(void) {
    static uint8_t current_digit_idx = 0;
    display_write_pos(current_digits[current_digit_idx], current_digit_idx);
    current_digit_idx = (current_digit_idx + 1) % 4;
}

static void counter_increment_callback(void) {
    counter++;
    if (counter > 9999) {
        counter = 0;
    }

    current_digits[0] = (counter / 1000) % 10;
    current_digits[1] = (counter / 100) % 10;
    current_digits[2] = (counter / 10) % 10;
    current_digits[3] = counter % 10;
}

void pot_to_digits_init(void) {
    display_init(); 
    display_timer = timer_new(2, display_multiplex_callback);
    counter_timer = timer_new(500, counter_increment_callback);
}

void pot_to_digits_start(void) {
    timer_start(display_timer);
    timer_start(counter_timer);
    counter = 0;
    is_puzzle_solved = false;
}

void pot_to_digits_stop(void) {
    timer_stop(display_timer);
    timer_stop(counter_timer);
}

bool pot_to_digits_is_solved(void) {
    return is_puzzle_solved;
}
