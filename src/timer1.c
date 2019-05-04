

#include <avr/io.h>
#include <avr/interrupt.h>
#include "timer1.h"
#include "motor.h"
#include "led.h"

#ifndef F_CPU
    #error F_CPU is not defined!
#endif

#define F_OCR1B_HZ 100

#define PRESCALER 1024
//#define VAL_OCR1B ( (F_CPU/(2*PRESCALER*F_OCR1B_HZ))-1)


void timer1_init() {
    DDRB |= (1<<PB1);       // set as output

    TCCR1B |= (1<<WGM12);            // CTC Mode
    TCCR1B |= (1<CS12) | (1<<CS10);  // prescaler 1024

    timer1_set_OCR1A(255);
    TIMSK1 |= (1<<OCIE1A);  // enable compare interrupt
}


void timer1_set_OCR1A(uint16_t value) {
    cli();
    OCR1AH = (uint8_t) (value>>8);
    OCR1AL = (uint8_t) value;
    sei();
}


ISR(TIMER1_COMPA_vect) {
    motor_cycle();
}

