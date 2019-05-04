#include <avr/io.h>
#include <stdio.h>
#include "menue.h"
#include "ssd1306.h"

#define MENUE_X_NAME  12
#define MENUE_X_VALUE 100

#define MENUE_Y_POWER   0
#define MENUE_Y_CLOCK   1
#define MENUE_Y_TIMEON  2
#define MENUE_Y_TIMEOFF 3

#define MENUE_Y_MAX     3

// 1 = true | 0 = false
#define MENUE_INVERT_DIRECTION 1




//static uint8_t g_menue_power = 0;
//static uint8_t g_menue_clock = 0;
//static uint8_t g_menue_timeon = 0;
//static uint8_t g_menue_timeoff = 0;



static void draw_value(uint8_t line, uint8_t value) {
    char buffer[4];
    sprintf(buffer, "%3d", value);
    ssd1306_puts(MENUE_X_VALUE, line, buffer);
}

static void draw_cursor(uint8_t line) {
    ssd1306_puts(0, line, "->");
}

static void clear_cursor(uint8_t line) {
    ssd1306_puts(0, line, "  ");
}

static void handle_power(int8_t delta) {
    static uint8_t power = 0;

    power += delta;
    draw_value(MENUE_Y_POWER, power);
}

static void handle_clock(int8_t delta) {
    static uint8_t clock = 0;

    clock += delta;
    draw_value(MENUE_Y_CLOCK, clock);
}

static void handle_timeon(int8_t delta) {
    static uint8_t time_on = 0;

    time_on += delta;
    draw_value(MENUE_Y_TIMEON, time_on);
}

static void handle_timeoff(int8_t delta) {
    static uint8_t time_off = 0;

    time_off += delta;
    draw_value(MENUE_Y_TIMEOFF, time_off);
}

void menue_init() {
    ssd1306_puts(MENUE_X_NAME, MENUE_Y_POWER,   "Max Power....");
    ssd1306_puts(MENUE_X_NAME, MENUE_Y_CLOCK,   "Clock........");
    ssd1306_puts(MENUE_X_NAME, MENUE_Y_TIMEON,  "Time On......");
    ssd1306_puts(MENUE_X_NAME, MENUE_Y_TIMEOFF, "Time Off.....");

    draw_cursor(MENUE_Y_POWER);
    handle_clock(0);
    handle_power(0);
    handle_timeon(0);
    handle_timeoff(0);
}


//void menue_set_power(uint8_t value) {
//    char buffer[4];
//    sprintf(buffer, "%3d", value);
//    ssd1306_puts(MENUE_X_VALUE, MENUE_Y_POWER, buffer);
//}

//void menue_set_clock(uint8_t value) {
//    char buffer[4];
//    sprintf(buffer, "%3d", value);
//    ssd1306_puts(MENUE_X_VALUE, MENUE_Y_CLOCK, buffer);
//}

//void menue_set_timeon(uint8_t value) {
//    char buffer[4];
//    sprintf(buffer, "%3d", value);
//    ssd1306_puts(MENUE_X_VALUE, MENUE_Y_TIMEON, buffer);
//}

//void menue_set_timeoff(uint8_t value) {
//    char buffer[4];
//    sprintf(buffer, "%3d", value);
//    ssd1306_puts(MENUE_X_VALUE, MENUE_Y_TIMEOFF, buffer);
//}

//uint8_t menue_get_power() {

//}

//uint8_t menue_get_clock() {

//}

//uint8_t menue_get_timeon() {

//}

//uint8_t menue_get_timeoff() {

//}


static uint8_t menue_move(int8_t direction) {
    static uint8_t current_line = 0;

    #if MENUE_INVERT_DIRECTION
        direction *= (-1);
    #endif


    if( direction == 0 ) {
        return current_line;
    }
    else if( direction > 0 ) {
        clear_cursor(current_line);

        if( current_line < MENUE_Y_MAX ) {
            current_line++;
        }
        else {
            current_line = 0;
        }

    }
    else if( direction < 0 ) {
        ssd1306_puts(0, current_line, "  ");

        if( current_line > 0 ) {
            current_line--;
        }
        else {
            current_line = MENUE_Y_MAX;
        }

    }

    draw_cursor(current_line);

    return current_line;
}

static void menue_edit(uint8_t line, int8_t encoder_value) {

    switch(line) {

    case MENUE_Y_POWER:
        handle_power(encoder_value);
        break;
    case MENUE_Y_CLOCK:
        handle_clock(encoder_value);
        break;
    case MENUE_Y_TIMEON:
        handle_timeon(encoder_value);
        break;
    case MENUE_Y_TIMEOFF:
        handle_timeoff(encoder_value);
        break;
    default:
        break;
    }


}






void menue_refresh(uint8_t mode, int8_t encoder_value) {

    static uint8_t current_line = 0;

    ssd1306_putc(0, 7, mode+'0');

    if( mode == MENUE_MODE_VIEW ) {
        current_line = menue_move(encoder_value);
    }
    else {
        menue_edit(current_line, encoder_value);
    }

}



