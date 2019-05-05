#ifndef TIMER2_H
#define TIMER2_H

#include <avr/io.h>

extern volatile uint8_t g_timer2_100ms;

void timer2_init();
uint8_t timer2_100ms_counter();

#endif // TIMER2_H
