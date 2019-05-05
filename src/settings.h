#ifndef SETTINGS_H
#define SETTINGS_H

#include <avr/io.h>

#define MENUE_MODE_EDIT 0
#define MENUE_MODE_VIEW 1


struct Settings {
    uint8_t motor_power;
    uint8_t motor_ontime;
    uint8_t motor_offtime;
    uint8_t menue_mode;
};



void settings_load(struct Settings *settings);
void settings_save(struct Settings *settings);




#endif // SETTINGS_H
