#include "motor.h"
#include <avr/io.h>
#include <avr/interrupt.h>

#ifndef F_CPU
    #error F_CPU is not defined!
#endif


#define MOTOR_DDRX  DDRB
#define MOTOR_BIT   PB1


void motor_init() {
    MOTOR_DDRX |= (1<<MOTOR_BIT);

    TCCR1A |= (1<<WGM10);               // 8bit PWM-Mode
    TCCR1A |= (1<<COM1A1);              // clear OC1A on compare
    TCCR1B |= (1<<CS11) | (1<<CS10);    // prescaler 64

    OCR1A = 0;
}


void motor_power(uint8_t power) {
    OCR1A = power;
}


