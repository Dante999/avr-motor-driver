#include <avr/io.h>
#include <avr/interrupt.h>
#include "led.h"

#ifndef F_CPU
    #error F_CPU is not defined!
#endif

#define F_PWM 100
// Prescaler 1, 8, 64, 256, 1024
#define PRESCALER 1024

//#define VAL_OCR1A ( (F_CPU/(2*PRESCALER*F_PWM))-1) )

static volatile uint8_t g_counter = 0;
static volatile uint8_t g_power = 0;


void motor_init() {
    DDRB |= (1<<PB1);       // set as output

    TCCR1B |= (1<<WGM12);   // CTC Mode


    // prescaler 1024
    TCCR1B |= (1<CS12) | (1<<CS10);

    motor_frequency(100);
    TIMSK1 |= (1<<OCIE1A);  // enable compare interrupt
}



ISR(TIMER1_COMPA_vect) {
    g_counter++;

    if( g_counter < g_power ) {
        motor_start();
    }
    else {
        motor_stop();
    }

    //led_toggle();
    //PORTB ^= (1<<PB1);
}

void motor_frequency(uint16_t frequency) {
    cli();
    OCR1AH = (uint8_t) (frequency>>8);
    OCR1AL = (uint8_t) frequency;
    sei();
}

void motor_power(uint8_t power) {
    g_power = power;
}

void motor_start() {
    PORTB |= (1<<PB1);
}

void motor_stop() {
    PORTB &= ~(1<<PB1);
}
