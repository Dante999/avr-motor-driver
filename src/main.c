#include <avr/io.h>
#include <util/delay.h>
#include "led.h"
#include "uart.h"
#include "ssd1306.h"


static void init() {
    led_init();
    uart_init();
    ssd1306_init();
}

int main() {

    init();

    ssd1306_fill(0x33);
    while(1) {
        _delay_ms(200);
        led_toggle();
        uart_putsln("test");
    }


}
