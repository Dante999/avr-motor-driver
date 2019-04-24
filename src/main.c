#include <avr/io.h>
#include <util/delay.h>
#include "led.h"
#include "uart.h"


static void init() {
    led_init();
    uart_init();
}

int main() {

    init();


    while(1) {
        _delay_ms(200);
        led_toggle();
        uart_putsln("test");
    }


}
