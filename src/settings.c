
#include "settings.h"




void settings_load(struct Settings *psettings) {

    psettings->time_left = 0;

    psettings->flags &= FLAG_MOTOR_RUNNING;
    psettings->flags &= FLAG_EDIT_MODE;
    psettings->flags &= FLAG_SYSTEM_ACTIVE;
    psettings->motor_power   = 200;
    psettings->motor_ontime  = 5;
    psettings->motor_offtime = 20;
}

void settings_save(struct Settings *psettings) {
    // save to eeprom at some time...
}
