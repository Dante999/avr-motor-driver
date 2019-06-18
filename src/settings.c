
#include "settings.h"




void settings_load(struct Settings *psettings) {

    psettings->time_left = 0;

    psettings->flags &= FLAG_MOTOR_RUNNING;
    psettings->flags &= FLAG_EDIT_MODE;
    psettings->motor_power   = 0;
    psettings->motor_ontime  = 10;
    psettings->motor_offtime = 50;
}

void settings_save(struct Settings *psettings) {
    // save to eeprom at some time...
}
