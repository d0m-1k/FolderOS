#pragma once

#include "types.h"

#define PIT_BASE_FREQUENCY 1193182
#define PIT_FREQUENCY 1000

void timer_handler();
void sleep(uint64_t milliseconds);
void init_timer();

extern volatile uint64_t timer_ticks;