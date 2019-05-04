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
    timer0_init();
    menue_init();
}






static uint8_t toggle_mode() {
    static uint8_t current_mode = MENUE_MODE_VIEW;

    if( current_mode == MENUE_MODE_VIEW ) {
        led_set_on();
        current_mode = MENUE_MODE_EDIT;
    }
    else {
        led_set_off();
        current_mode = MENUE_MODE_VIEW;
    }

    return current_mode;
}




int main() {

    init();
    sei();

    int8_t encoder_val = 0;
    uint8_t switch_old = 0;
    uint8_t switch_new = 0;

    uint8_t mode = MENUE_MODE_VIEW;


    while(1) {

        switch_new = encoder_switch();

        if( (switch_new == 1) && (switch_old == 0) ) {
            mode = toggle_mode();
        }

        encoder_val += encoder_read();
        menue_refresh(mode, encoder_val);
        encoder_val = 0;

        switch_old = switch_new;

    }


}
