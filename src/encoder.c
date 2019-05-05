/************************************************************************/
/*                                                                      */
/*                      Reading rotary encoder                      */
/*                      one, two and four step encoders supported   */
/*                                                                      */
/*              Author: Peter Dannegger                                 */
/*                                                                      */
/************************************************************************/
#include "encoder.h"
#include <avr/interrupt.h>




/*
 * define the type of encoder (valid values are 1, 2 and 4)
 */
#define ENCODER_STEPS 2

#define SWITCH_DEBOUNCE 5

#define SWITCH      (PINC & 1<<PC1)
#define PHASE_A     (PINC & 1<<PC2)
#define PHASE_B     (PINC & 1<<PC3)


static struct EncoderStats{
    volatile int8_t delta;
    int8_t last;
    uint8_t pressed;
} g_stats;


static void init_timer() {
    TCCR0A |= (1<<WGM01);                   // CTC Mode
    TCCR0B |= (1<<CS01) | (1<<CS00);        // prescaler 64

    OCR0A = (uint8_t)(F_CPU / 64.0 * 1e-3 - 0.5);   // 1ms
    TIMSK0 |= (1<<OCIE0A);                  // enable interrupt
}


void encoder_init() {

    init_timer();

    int8_t new;

    new = 0;
    if( PHASE_A ) new = 3;
    if( PHASE_B ) new ^= 1;                 // convert gray to binary

    g_stats.last = new;                     // power on state
    g_stats.delta = 0;
}


ISR( TIMER0_COMPA_vect ) {
    static uint8_t counter = 0;

    counter++;

    int8_t new, diff;

    new = 0;
    if( PHASE_A ) new = 3;
    if( PHASE_B ) new ^= 1;                 // convert gray to binary
    diff = g_stats.last - new;              // difference last - new
    if( diff & 1 ) {                        // bit 0 = value (1)
        g_stats.last = new;                 // store new as next last
        g_stats.delta += (diff & 2) - 1;    // bit 1 = direction (+/-)
    }


    if( counter >= SWITCH_DEBOUNCE ) {

        if( SWITCH ) {
            g_stats.pressed = 0;
        }
        else {
            g_stats.pressed = 1;
        }

        counter = 0;
    }
}


uint8_t encoder_switch() {
    return g_stats.pressed;
}



#if ENCODER_STEPS == 1
    int8_t encoder_read() {         // read single step encoders

        int8_t val;

        cli();
        val = enc_delta;
        enc_delta = 0;
        sei();
        return val;                   // counts since last call
    }

#elif ENCODER_STEPS == 2
    int8_t encoder_read() {         // read two step encoders

        int8_t val;

        cli();
        val = g_stats.delta;
        g_stats.delta = val & 1;
        sei();
        return val >> 1;
    }

#elif ENCODER_STEPS == 4
    int8_t encoder_read() {         // read four step encoders

        int8_t val;

        cli();
        val = enc_delta;
        enc_delta = val & 3;
        sei();
        return val >> 2;
    }

#else
    #error "Invalid parameter ENCODER_STEPS set!"
#endif



