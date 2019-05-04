#include "motor.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include "timer1.h"

#ifndef F_CPU
    #error F_CPU is not defined!
#endif

#define F_PWM 100
// Prescaler 1, 8, 64, 256, 1024
#define PRESCALER 1024

//#define VAL_OCR1A ( (F_CPU/(2*PRESCALER*F_PWM))-1) )


#define MOTOR_DDRX  DDRB
#define MOTOR_PORTX PORTB
#define MOTOR_BIT   PB1



static volatile uint8_t g_counter = 0;
static volatile uint8_t g_power = 0;

static void motor_start();
static void motor_stop();

void motor_init() {
    timer1_init();
    MOTOR_DDRX |= (1<<MOTOR_BIT);
    motor_frequency(100);
}


void motor_frequency(uint16_t frequency) {
    timer1_set_OCR1A(frequency);
}


void motor_power(uint8_t power) {
    g_power = power;
}


void motor_cycle() {
    g_counter++;

    if( g_counter < g_power ) {
        motor_start();
    }
    else {
        motor_stop();
    }
}


static void motor_start() {
    MOTOR_PORTX |= (1<<MOTOR_BIT);
}


static void motor_stop() {
    MOTOR_PORTX &= ~(1<<MOTOR_BIT);
}


