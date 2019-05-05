#include "timer2.h"
#include <avr/interrupt.h>
#include "led.h"






#ifndef F_CPU
    #error "F_CPU is not defined!"
#endif

#define PRESCALER   1024UL
#define F_CTC       100UL



#define VAL (F_CPU/(PRESCALER*F_CTC))

#if  VAL < 0
    #error "Register value for Timer2 is smaller than 0!"
#elif VAL > 255
    #error "Register value for Timer2 is greater than 255!" #VAL_OCR
#endif




volatile uint8_t g_timer2_100ms = 0;


void timer2_init() {
    TCCR2A |= (1<<WGM21);           // CTC-Mode


    #if PRESCALER == 1024
        TCCR2B |= (1<<CS22) | (1<<CS21) | (1<<CS20);    // prescaler 1024
    #elif PRESCALER == 64
        TCCR2B |= (1<<CS22);                            // prescaler 64
    #else
        #error "No valid prescaler set for timer2!"
    #endif

    OCR2A = (uint8_t) (VAL-0.5);    // calculate compare register value

    TIMSK2 |= (1<<OCIE2A);          // enable interrupt

}


uint8_t timer2_100ms_counter() {
    return g_timer2_100ms;
}



ISR(TIMER2_COMPA_vect) {

    static uint8_t i = 0;

    i++;

    if( i%10 == 0 ) {
        g_timer2_100ms++;
    }
}
