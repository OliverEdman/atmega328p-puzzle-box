/**
 * @file timer.c
 * @brief Implementation av en inkapslad mjukvarutimer-modul för ATmega328P.
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#include "timer.h"

#define MAX_TIMERS 3

//  (Opaque)
struct timer {
    uint32_t timeout_ms;       // Måltid för timern
    timer_callback_t callback;  // Funktion som anropas vid timeout
    bool is_enabled;            // Om timern aktivt räknar
    bool in_use;                // Om platsen i poolen är upptagen
    uint32_t elapsed_ms;        // Ackumulerad tid
};

// Statisk pool för att undvika dynamisk minnesallokering (malloc)
static struct timer timer_pool[MAX_TIMERS];

static volatile uint32_t system_ticks = 0;


void timer_init(void) {
    /* Nollställ poolen för säkerhets skull */
    for (int i = 0; i < MAX_TIMERS; i++) {
        timer_pool[i].in_use = false;
        timer_pool[i].is_enabled = false;
    }

    /* Timer0 på ATmega328P */
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

timer_t* timer_new(uint32_t timeout_ms, timer_callback_t callback) {
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

void timer_delete(timer_t** self) {
    if (self == NULL || *self == NULL) {
        return;
    }

    (*self)->in_use = false;
    (*self)->is_enabled = false;
    (*self)->elapsed_ms = 0;
    *self = NULL; 
}

bool timer_is_enabled(const timer_t* self) {
    return (self != NULL) ? self->is_enabled : false;
}

void timer_start(timer_t* self) {
    if (self != NULL) {
        self->is_enabled = true;
    }
}

void timer_stop(timer_t* self) {
    if (self != NULL) {
        self->is_enabled = false;
    }
}

void timer_toggle(timer_t* self) {
    if (self != NULL) {
        self->is_enabled = !self->is_enabled;
    }
}

void timer_restart(timer_t* self) {
    if (self != NULL) {
        self->elapsed_ms = 0;
        self->is_enabled = true;
    }
}

void timer_reset(timer_t* self) {
    if (self != NULL) {
        self->elapsed_ms = 0;
        self->is_enabled = false;
    }
}

uint32_t timer_timeout_ms(const timer_t* self) {
    if (self == NULL) {
        return 0;
    }
    
    return self->timeout_ms;
}

void timer_set_timeout_ms(timer_t* self, uint32_t timeout_ms) {
    if (self != NULL && timeout_ms > 0) {
        self->timeout_ms = timeout_ms;
        self->elapsed_ms = 0;
    }
}
