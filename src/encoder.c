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
#include "led.h"

// valid: 1, 2, 4
#define ENCODER_STEPS 2
#define SWITCH_DEBOUNCE 5

#define SWITCH      (PINC & 1<<PC1)
#define PHASE_A     (PINC & 1<<PC2)
#define PHASE_B     (PINC & 1<<PC3)



volatile int8_t enc_delta;          // -128 ... 127
volatile uint8_t g_encoder_pressed;

static int8_t last;


static void init_timer() {
    TCCR0A |= (1<<WGM01);
    TCCR0B |= (1<<CS01) | (1<<CS00);   // CTC, prescaler 64

    OCR0A = (uint8_t)(F_CPU / 64.0 * 1e-3 - 0.5);   // 1ms
    TIMSK0 |= (1<<OCIE0A);
}

ISR( TIMER0_COMPA_vect ) {
    encoder_calc();
}


void encoder_init( void )
{

    init_timer();

  int8_t new;

  new = 0;
  if( PHASE_A ) new = 3;
  if( PHASE_B ) new ^= 1;       // convert gray to binary
  last = new;                   // power on state
  enc_delta = 0;
}


void encoder_calc()
{
    static uint8_t counter = 0;

    counter++;

  int8_t new, diff;

  new = 0;
  if( PHASE_A ) new = 3;
  if( PHASE_B ) new ^= 1;          // convert gray to binary
  diff = last - new;               // difference last - new
  if( diff & 1 ) {                 // bit 0 = value (1)
    last = new;                    // store new as next last
    enc_delta += (diff & 2) - 1;   // bit 1 = direction (+/-)
  }


  if( counter >= SWITCH_DEBOUNCE ) {

      if( SWITCH ) {
          g_encoder_pressed = 0;
      }
      else {
          g_encoder_pressed = 1;
      }

      counter = 0;
  }



}


#if ENCODER_STEPS == 1
int8_t encoder_read( void )         // read single step encoders
{
  int8_t val;

  cli();
  val = enc_delta;
  enc_delta = 0;
  sei();
  return val;                   // counts since last call
}


#elif ENCODER_STEPS == 2
int8_t encoder_read( void )         // read two step encoders
{
  int8_t val;

  cli();
  val = enc_delta;
  enc_delta = val & 1;
  sei();
  return val >> 1;
}


#elif ENCODER_STEPS == 4
int8_t encoder_read( void )         // read four step encoders
{
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

uint8_t encoder_switch() {
    return g_encoder_pressed;
}

