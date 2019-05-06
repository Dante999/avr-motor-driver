#ifndef SETTINGS_H
#define SETTINGS_H

#include <avr/io.h>

#define MENUE_MODE_EDIT 0
#define MENUE_MODE_VIEW 1

#define MOTOR_STATE_RUNNING 1
#define MOTOR_STATE_STOPPED 0

struct Settings {
    uint8_t motor_power;
    uint8_t motor_ontime;
    uint8_t motor_offtime;
    uint8_t menue_mode;
    uint8_t time_left;
    uint8_t motor_state;
};



void settings_load(struct Settings *settings);
void settings_save(struct Settings *settings);




#endif // SETTINGS_H
