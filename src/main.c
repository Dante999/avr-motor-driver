#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include "led.h"
#include "uart.h"
#include "ssd1306.h"
#include "motor.h"
#include "graphx.h"
#include "encoder.h"
#include "timer0.h"
#include "menue.h"





static void init() {
    led_init();
    uart_init();
    ssd1306_init();
    motor_init();
    encoder_init();
    menue_init();
}







//static void time_encoder(uint8_t ms_counter) {
//    static uint8_t old_ms = 0;


//    if( ms_counter != old_ms ) {
//        encoder_calc();
//    }

//    old_ms = ms_counter;
//}

//static void handle_timing() {

//    uint8_t ms_counter = timer0_get_counter();

//    time_encoder(ms_counter);

//}


static uint8_t toggle_mode() {
    static uint8_t current_mode = MENUE_MODE_VIEW;

    if( current_mode == MENUE_MODE_VIEW ) {
        current_mode = MENUE_MODE_EDIT;
    }
    else {
        current_mode = MENUE_MODE_VIEW;
    }

    return current_mode;
}

static void handle_menue() {
    static int8_t encoder_val = 0;
    static uint8_t switch_old = 0;
    static uint8_t switch_new = 0;
    static uint8_t mode = MENUE_MODE_VIEW;

    switch_new = encoder_switch();

    if( (switch_new == 1) && (switch_old == 0) ) {
        mode = toggle_mode();
    }

    encoder_val += encoder_read();
    menue_refresh(mode, encoder_val);
    encoder_val = 0;

    switch_old = switch_new;
}



int main() {

    init();
    sei();




    while(1) {
//        handle_timing();
        handle_menue();
    }


}
