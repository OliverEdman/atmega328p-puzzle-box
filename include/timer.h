#ifndef TIMER_H_
#define TIMER_H_

#include <stdint.h>

void timer_init(void);
uint32_t time_ms(void);


#define current_time() time_ms()

#endif