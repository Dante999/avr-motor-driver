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



/**
 * @brief initializes all relevant modules
 */
static void init() {
    led_init();
    uart_init();
    ssd1306_init();
    motor_init();
    timer2_init();
}



static void handle_system_active(struct Settings *psettings) {

    uint8_t counter_100ms = g_timer2_100ms;


    if( psettings->flags & FLAG_MOTOR_RUNNING ) {

        led_set_on();

        if( counter_100ms < psettings->motor_ontime ) {
            psettings->time_left = psettings->motor_ontime-counter_100ms;
            motor_power(psettings->motor_power);
        }
        else {
            psettings->flags &= ~(FLAG_MOTOR_RUNNING);
            motor_power(0);
            g_timer2_100ms = 0;
        }
    }
    else {

        led_set_off();

        if( counter_100ms < psettings->motor_offtime ) {
            psettings->time_left = psettings->motor_offtime-counter_100ms;
            motor_power(0);
        }
        else {
            psettings->flags |= (FLAG_MOTOR_RUNNING);
            motor_power(psettings->motor_power);
            g_timer2_100ms = 0;
        }
    }

}

static void handle_system_inactive(struct Settings *psettings) {
    led_set_off();
}


/**
 * @brief organizes timing stuff
 *
 * @param psettings   pointer to the settings structure
 */
static void scheduler(struct Settings *psettings) {

    if( psettings->flags & FLAG_SYSTEM_ACTIVE ) {
        handle_system_active(psettings);
    }
    else {
        handle_system_inactive(psettings);
    }

}



/**
 * @brief main
 * @return
 */
int main() {

    init();
    sei();

    struct Settings setting;

    settings_load(&setting);

    menue_init(&setting);

    while(1) {
        menue_refresh(&setting);
        scheduler(&setting);
    }


}
