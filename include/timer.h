#ifndef TIMER_H_
#define TIMER_H_

#include <stdint.h>

// Initierar hårdvarutimern (Timer0) för 1ms tick
void timer_init(void);

// Hämtar systemtiden i millisekunder (atomär läsning)
uint32_t timer_get_millis(void);

// Blockerande fördröjning
void timer_delay_ms(uint32_t ms);

#endif