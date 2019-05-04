#include <avr/io.h>
#include <avr/interrupt.h>
#include "encoder.h"

void timer0_init() {
    TCCR0A |= (1<<WGM01);
    TCCR0B |= (1<<CS01) | (1<<CS00);   // CTC, prescaler 64

    OCR0A = (uint8_t)(F_CPU / 64.0 * 1e-3 - 0.5);   // 1ms
    TIMSK0 |= (1<<OCIE0A);
}


ISR( TIMER0_COMPA_vect ) {
    encoder_calc();
}
