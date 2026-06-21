/**
 * @file timer.c
 * @brief Implementation of the timer driver.
 * @author Oliver Edman <o.edman@icloud.com>
 *
 * @note Licensed under the MIT License.
 */

#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#include "timer.h"

#define MAX_TIMERS 3 // change when needed

//  (Opaque)
struct timer {
	uint32_t timeout_ms;
	timer_callback_t callback;
	bool is_enabled;
	bool in_use;
	uint32_t elapsed_ms;
};

static struct timer timer_pool[MAX_TIMERS];

volatile uint32_t system_ticks = 0;

void timer_init(void) {

    for (int i = 0; i < MAX_TIMERS; i++) {
        timer_pool[i].in_use = false;
        timer_pool[i].is_enabled = false;
    }

    /* Timer0 on ATmega328P */
    TCCR0A = (1 << WGM01);              // CTC-läge (Clear Timer on Compare)
    TCCR0B = (1 << CS01) | (1 << CS00); // Prescaler 64 (16MHz / 64 = 250kHz)
    OCR0A = 249;                        // (250kHz / 1000Hz) - 1 = 249 för 1ms
    TIMSK0 = (1 << OCIE0A);             // Aktivera avbrott för Compare Match A
    
    sei();                              // Aktivera globala avbrott
}

/**
 * @brief Interrupt Service Routine (ISR) för Timer0.
 * Körs exakt varje millisekund.
 */
ISR(TIMER0_COMPA_vect) {
    system_ticks++;
}

/**
 * @brief Bakgrundshanterare för timers.
 * Ska anropas i main-loopens while(1).
 */
void timer_handler(void) {
    static uint32_t last_ticks = 0;
    
    cli();
    uint32_t current_ticks = system_ticks;
    sei();

    if (current_ticks != last_ticks) {
        uint32_t diff = current_ticks - last_ticks;
        last_ticks = current_ticks;

        for (int i = 0; i < MAX_TIMERS; i++) {
            if (timer_pool[i].in_use && timer_pool[i].is_enabled) {
                timer_pool[i].elapsed_ms += diff;

                if (timer_pool[i].elapsed_ms >= timer_pool[i].timeout_ms) {
                    timer_pool[i].elapsed_ms = 0; // Återställ räknare
                    
                    if (timer_pool[i].callback != NULL) {
                        timer_pool[i].callback(); // Kör callback
                    }
                }
            }
        }
    }
}

struct timer* timer_new(uint32_t timeout_ms, timer_callback_t callback) {
    if (timeout_ms == 0 || callback == NULL) {
        return NULL;
    }

    for (int i = 0; i < MAX_TIMERS; i++) {
        if (!timer_pool[i].in_use) {
            timer_pool[i].in_use = true;
            timer_pool[i].timeout_ms = timeout_ms;
            timer_pool[i].callback = callback;
            timer_pool[i].is_enabled = false; // Kräver timer_start()
            timer_pool[i].elapsed_ms = 0;
            return &timer_pool[i];
        }
    }
    return NULL;
}

void timer_delete(struct timer** self) {
    if (self == NULL || *self == NULL) {
        return;
    }

    (*self)->in_use = false;
    (*self)->is_enabled = false;
    (*self)->elapsed_ms = 0;

    *self = NULL; 
}

bool timer_is_enabled(const struct timer* self) {
    return (self != NULL) ? self->is_enabled : false;
}

void timer_start(struct timer* self) {
    if (self != NULL) {
        self->is_enabled = true;
    }
}

void timer_stop(struct timer* self) {
    if (self != NULL) {
        self->is_enabled = false;
    }
}

void timer_toggle(struct timer* self) {
    if (self != NULL) {
        self->is_enabled = !self->is_enabled;
    }
}

void timer_restart(struct timer* self) {
    if (self != NULL) {
        self->elapsed_ms = 0;
        self->is_enabled = true;
    }
}

void timer_reset(struct timer* self) {
    if (self != NULL) {
        self->elapsed_ms = 0;
        self->is_enabled = false;
    }
}

uint32_t timer_timeout_ms(const struct timer* self) {
    if (self == NULL) {
        return 0;
    }
    
    return self->timeout_ms;
}

void timer_set_timeout_ms(struct timer* self, uint32_t timeout_ms) {
    if (self != NULL && timeout_ms > 0) {
        self->timeout_ms = timeout_ms;
        self->elapsed_ms = 0;
    }
}
