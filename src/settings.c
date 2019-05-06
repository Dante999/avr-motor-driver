
#include "settings.h"




void settings_load(struct Settings *settings) {
    settings->menue_mode = MENUE_MODE_VIEW;
    settings->time_left = 0;

    settings->motor_state   = MOTOR_STATE_STOPPED;
    settings->motor_power   = 0;
    settings->motor_ontime  = 10;
    settings->motor_offtime = 50;
}

void settings_save(struct Settings *settings) {
    // save to eeprom at some time...
}
