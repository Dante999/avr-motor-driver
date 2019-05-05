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





#define STATE_MOTOR_ON 1
#define STATE_MOTOR_OFF 0

static void scheduler(struct Settings *settings) {

    static uint8_t motor_state = STATE_MOTOR_ON;

    if( motor_state == STATE_MOTOR_ON ) {

        led_set_on();

        if( g_timer2_100ms < settings->motor_ontime ) {
            motor_power(settings->motor_power);
        }
        else {
            motor_power(0);
            motor_state = STATE_MOTOR_OFF;
            g_timer2_100ms = 0;
        }
    }
    else {

        led_set_off();

        if( g_timer2_100ms < settings->motor_offtime ) {
            motor_power(0);
        }
        else {
            motor_power(settings->motor_power);
            motor_state = STATE_MOTOR_ON;
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
