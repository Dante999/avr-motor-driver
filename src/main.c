#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include "led.h"
#include "uart.h"
#include "ssd1306.h"
#include "motor.h"
#include "encoder.h"
#include "menue.h"
#include "timer2.h"
#include "settings.h"

static void init() {
    led_init();
    uart_init();
    ssd1306_init();
    motor_init();
    timer2_init();
}





static void scheduler(struct Settings *settings) {

    uint8_t counter_100ms = g_timer2_100ms;


    if( settings->motor_state == MOTOR_STATE_RUNNING ) {

        led_set_on();

        if( counter_100ms < settings->motor_ontime ) {
            settings->time_left = settings->motor_ontime-counter_100ms;
            motor_power(settings->motor_power);
        }
        else {
            settings->motor_state = MOTOR_STATE_STOPPED;
            motor_power(0);
            g_timer2_100ms = 0;
        }
    }
    else {

        led_set_off();

        if( counter_100ms < settings->motor_offtime ) {
            settings->time_left = settings->motor_offtime-counter_100ms;
            motor_power(0);
        }
        else {
            motor_power(settings->motor_power);
            settings->motor_state = MOTOR_STATE_RUNNING;
            g_timer2_100ms = 0;
        }
    }

}




int main() {

    init();
    sei();

    struct Settings Setting;

    settings_load(&Setting);

    menue_init(&Setting);

    while(1) {
        menue_refresh(&Setting);
        scheduler(&Setting);
    }


}
