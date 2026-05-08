#ifndef TIMER_H_
#define TIMER_H_

#include <stdbool.h>
#include <stdint.h>

typedef struct timer timer_t;
typedef void (*timer_callback_t)(void);


void timer_init(void);


timer_t* timer_new(uint32_t timeout_ms, timer_callback_t callback);
void timer_delete(timer_t** self);


void timer_start(timer_t* self);
void timer_stop(timer_t* self);
void timer_restart(timer_t* self);

#endif
