#include "menue.h"
#include <avr/io.h>
#include <stdio.h>
#include "ssd1306.h"
#include "encoder.h"
#include "version.h"
#include "bitmap.h"

#define MENUE_X_MIN    0
#define MENUE_X_NAME  12
#define MENUE_X_VALUE 80
#define MENUE_X_STATE 15


#define MENUE_Y_MIN     2
#define MENUE_Y_POWER   2
#define MENUE_Y_TIMEON  3
#define MENUE_Y_TIMEOFF 4
#define MENUE_Y_SYSTEM  5
#define MENUE_Y_MAX     5

#define MENUE_Y_STATUS   7
#define ANIMATION_DELAY 10




// 1 = true | 0 = false
#define MENUE_INVERT_DIRECTION 1

static void draw_value(uint8_t line, uint8_t value) {
    char buffer[4];
    sprintf(buffer, "%03d", value);
    uint8_t col = ssd1306_puts(MENUE_X_VALUE, line, "  ");
    ssd1306_puts(col, line, buffer);
}

static void draw_floating_value(uint8_t line, uint8_t value) {

    uint8_t x_coordinate = MENUE_X_VALUE;
    char buffer[4];
    sprintf(buffer, "%03d", value);

    x_coordinate = ssd1306_putc(x_coordinate, line, ' ');

    if( buffer[0] != '0' ) {
        x_coordinate = ssd1306_putc(x_coordinate, line, buffer[0]);
    }
    else {
        x_coordinate = ssd1306_putc(x_coordinate, line, ' ');
    }

    x_coordinate = ssd1306_putc(x_coordinate, line, buffer[1]);
    x_coordinate = ssd1306_putc(x_coordinate, line, '.');
    x_coordinate = ssd1306_putc(x_coordinate, line, buffer[2]);
    x_coordinate = ssd1306_putc(x_coordinate, line, 's');
}

static void draw_motor_power(struct Settings *settings) {
    draw_value(MENUE_Y_POWER, settings->motor_power);
}

static void draw_motor_ontime(struct Settings *settings) {
    draw_floating_value(MENUE_Y_TIMEON, settings->motor_ontime);
}

static void draw_motor_offtime(struct Settings *settings) {
    draw_floating_value(MENUE_Y_TIMEOFF, settings->motor_offtime);
}

static void draw_system_state(struct Settings *psettings) {

    if( psettings->flags & FLAG_SYSTEM_ACTIVE ) {
        ssd1306_puts(MENUE_X_VALUE, MENUE_Y_SYSTEM, "  PLAY");

    }
    else {
        ssd1306_puts(MENUE_X_VALUE, MENUE_Y_SYSTEM, " PAUSE");
    }

}

static void draw_status(struct Settings *psettings) {

    if( psettings->flags & FLAG_SYSTEM_ACTIVE ) {

        if( psettings->flags & FLAG_MOTOR_RUNNING ) {
            ssd1306_puts(MENUE_X_STATE, MENUE_Y_STATUS, "Running... ");
        }
        else {
            ssd1306_puts(MENUE_X_STATE, MENUE_Y_STATUS, "Stopping...");
        }

        draw_floating_value(MENUE_X_STATE, psettings->time_left);

    }
    else {
        ssd1306_puts(MENUE_X_STATE, MENUE_Y_STATUS, "Paused... ");
    }



//    static uint8_t i = 0;

//    i++;

//    uint8_t x = MENUE_X_STATE;

//    ssd1306_draw_bitmap(x+1*BITMAP_WIDTH, 7, bitmap_crankshaft, BITMAP_WIDTH);
//    ssd1306_draw_bitmap(x+3*BITMAP_WIDTH, 7, bitmap_crankshaft, BITMAP_WIDTH);
//    ssd1306_draw_bitmap(x+5*BITMAP_WIDTH, 7, bitmap_crankshaft, BITMAP_WIDTH);

//    if( (i%ANIMATION_DELAY==0) && (psettings->flags&FLAG_MOTOR_RUNNING) ) {
//        static uint8_t k = 0;
//        ssd1306_draw_bitmap(x+0*BITMAP_WIDTH, 6, bitmap_cylinder+k*BITMAP_WIDTH,       BITMAP_WIDTH);
//        ssd1306_draw_bitmap(x+0*BITMAP_WIDTH, 7, bitmap_connecting_rod+k*BITMAP_WIDTH, BITMAP_WIDTH);

//        k ^= (1<<0);
//        ssd1306_draw_bitmap(x+2*BITMAP_WIDTH, 6, (bitmap_cylinder+k*BITMAP_WIDTH),       BITMAP_WIDTH);
//        ssd1306_draw_bitmap(x+2*BITMAP_WIDTH, 7, (bitmap_connecting_rod+k*BITMAP_WIDTH), BITMAP_WIDTH);

//        k ^= (1<<0);
//        ssd1306_draw_bitmap(x+4*BITMAP_WIDTH, 6, bitmap_cylinder+k*BITMAP_WIDTH,       BITMAP_WIDTH);
//        ssd1306_draw_bitmap(x+4*BITMAP_WIDTH, 7, bitmap_connecting_rod+k*BITMAP_WIDTH, BITMAP_WIDTH);

//        k ^= (1<<0);
//        ssd1306_draw_bitmap(x+6*BITMAP_WIDTH, 6, bitmap_cylinder+k*BITMAP_WIDTH,       BITMAP_WIDTH);
//        ssd1306_draw_bitmap(x+6*BITMAP_WIDTH, 7, bitmap_connecting_rod+k*BITMAP_WIDTH, BITMAP_WIDTH);
//    }

//    draw_floating_value(7, psettings->time_left);

}

static void draw_view_cursor(uint8_t line) {
    ssd1306_puts(0, line, "->");
}

static void clear_view_cursor(uint8_t line) {
    ssd1306_puts(0, line, "  ");
}

static void draw_edit_cursor(uint8_t line) {
    ssd1306_puts(0, line, "x");
}

static void clear_edit_cursor(uint8_t line) {
    ssd1306_puts(0, line, "  ");
}

static uint8_t handle_view_mode() {
    static uint8_t current_line = MENUE_Y_MIN;

    int8_t direction = encoder_read();

    #if MENUE_INVERT_DIRECTION
        direction *= (-1);
    #endif


    if( direction == 0 ) {
        return current_line;
    }
    else if( direction > 0 ) {
        clear_view_cursor(current_line);

        if( current_line < MENUE_Y_MAX ) {
            current_line++;
        }
        else {
            current_line = MENUE_Y_MIN;
        }

    }
    else if( direction < 0 ) {
        ssd1306_puts(0, current_line, "  ");

        if( current_line > MENUE_Y_MIN ) {
            current_line--;
        }
        else {
            current_line = MENUE_Y_MAX;
        }

    }

    draw_view_cursor(current_line);

    return current_line;
}


static void handle_edit_mode(struct Settings *settings, uint8_t line) {

    int8_t encoder_val = encoder_read();

    switch(line) {

    case MENUE_Y_POWER:
        settings->motor_power += encoder_val;
        draw_motor_power(settings);
        break;
    case MENUE_Y_TIMEON:
        settings->motor_ontime += encoder_val;
        draw_motor_ontime(settings);
        break;
    case MENUE_Y_TIMEOFF:
        settings->motor_offtime += encoder_val;
        draw_motor_offtime(settings);
        break;
    case MENUE_Y_SYSTEM:

        if( encoder_val & 0x01 ) {
            settings->flags ^= FLAG_SYSTEM_ACTIVE; // toggle the bit
        }

        draw_system_state(settings);
        break;

    default:
        break;
    }


}

static void toggle_mode(struct Settings *psettings, uint8_t current_line) {

    if( psettings->flags & FLAG_EDIT_MODE ) {
        psettings->flags &= ~(FLAG_EDIT_MODE);
        clear_edit_cursor(current_line);
        draw_view_cursor(current_line);
    }
    else {
        psettings->flags |= (FLAG_EDIT_MODE);
        clear_view_cursor(current_line);
        draw_edit_cursor(current_line);
    }

}


static void handle_encoder_switch(struct Settings *settings, uint8_t current_line) {
    static uint8_t switch_old = 0;
    static uint8_t switch_new = 0;


    switch_new = encoder_switch();

    if( (switch_new == 1) && (switch_old == 0) ) {
        toggle_mode(settings, current_line);
    }

    switch_old = switch_new;
}

void menue_init(struct Settings *psettings) {
    encoder_init();

    uint8_t x = ssd1306_puts(0, 0, "AVR Motor Driver ");
    ssd1306_puts(x, 0, RELEASE_VERSION);

    ssd1306_puts(MENUE_X_MIN, 1,                "---------------------");
    ssd1306_puts(MENUE_X_NAME, MENUE_Y_POWER,   "Max Power : ");
    ssd1306_puts(MENUE_X_NAME, MENUE_Y_TIMEON,  "Time On   : ");
    ssd1306_puts(MENUE_X_NAME, MENUE_Y_TIMEOFF, "Time Off  : ");
    ssd1306_puts(MENUE_X_NAME, MENUE_Y_SYSTEM,  "System    : ");
    ssd1306_puts(0, MENUE_Y_MAX+1,              "---------------------");
    draw_view_cursor(MENUE_Y_POWER);
    draw_motor_power(psettings);
    draw_motor_ontime(psettings);
    draw_motor_offtime(psettings);
    draw_system_state(psettings);

    draw_status(psettings);
}


void menue_refresh(struct Settings *psettings) {
    static uint8_t current_line = MENUE_Y_MIN;

    handle_encoder_switch(psettings, current_line);

    if( psettings->flags & FLAG_EDIT_MODE ) {
        handle_edit_mode(psettings, current_line);
    }
    else {
        current_line = handle_view_mode();
    }

    draw_status(psettings);

}




