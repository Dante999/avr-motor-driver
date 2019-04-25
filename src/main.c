#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "led.h"
#include "uart.h"
#include "ssd1306.h"
#include "motor.h"

static void init() {
    led_init();
    uart_init();
    ssd1306_init();
    motor_init();
}

int main() {

    init();
    sei();

    ssd1306_fill(0x33);
    motor_power(0);


    while(1) {
    }


}
