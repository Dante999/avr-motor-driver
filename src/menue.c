#include "menue.h"
#include <avr/io.h>
#include <stdio.h>
#include "ssd1306.h"
#include "encoder.h"
#include "version.h"

#define MENUE_X_NAME  12
#define MENUE_X_VALUE 80



#define MENUE_Y_POWER   3
#define MENUE_Y_TIMEON  4
#define MENUE_Y_TIMEOFF 5
#define MENUE_Y_STATE   7

#define MENUE_Y_MIN     MENUE_Y_POWER
#define MENUE_Y_MAX     MENUE_Y_TIMEOFF


// 1 = true | 0 = false
#define MENUE_INVERT_DIRECTION 1

static void draw_value(uint8_t line, uint8_t value) {
    char buffer[4];
    sprintf(buffer, "%3d", value);
    ssd1306_puts(MENUE_X_VALUE, line, buffer);
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

static void draw_motor_state(struct Settings *psettings) {

    if( psettings->motor_state == MOTOR_STATE_RUNNING ) {
        ssd1306_puts(MENUE_X_NAME, MENUE_Y_STATE, "running...");
    }
    else {
        ssd1306_puts(MENUE_X_NAME, MENUE_Y_STATE, "stopped...");
    }

    draw_floating_value(7, psettings->time_left);

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
    default:
        break;
    }


}

static void toggle_mode(struct Settings *settings, uint8_t current_line) {

    if( settings->menue_mode == MENUE_MODE_VIEW ) {
        settings->menue_mode = MENUE_MODE_EDIT;
        clear_view_cursor(current_line);
        draw_edit_cursor(current_line);
    }
    else {
        settings->menue_mode = MENUE_MODE_VIEW;
        clear_edit_cursor(current_line);
        draw_view_cursor(current_line);
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

void menue_init(struct Settings *settings) {
    encoder_init();

    uint8_t x = ssd1306_puts(0, 0, "AVR Motor Driver ");
    ssd1306_puts(x, 0, RELEASE_VERSION);

    ssd1306_puts(0, 1, "---------------------");
    ssd1306_puts(MENUE_X_NAME, MENUE_Y_POWER,   "Max Power : ");
    ssd1306_puts(MENUE_X_NAME, MENUE_Y_TIMEON,  "Time On   : ");
    ssd1306_puts(MENUE_X_NAME, MENUE_Y_TIMEOFF, "Time Off  : ");

    draw_view_cursor(MENUE_Y_POWER);
    draw_motor_power(settings);
    draw_motor_ontime(settings);
    draw_motor_offtime(settings);

    draw_motor_state(settings);
}


void menue_refresh(struct Settings *settings) {
    static uint8_t current_line = MENUE_Y_MIN;

    handle_encoder_switch(settings, current_line);

    if( settings->menue_mode == MENUE_MODE_VIEW ) {
        current_line = handle_view_mode();
    }
    else {
        handle_edit_mode(settings, current_line);
    }

    draw_motor_state(settings);

}




