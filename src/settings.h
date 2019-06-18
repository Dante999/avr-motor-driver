#ifndef SETTINGS_H
#define SETTINGS_H

#include <avr/io.h>





#define FLAG_EDIT_MODE     (1<<0)
#define FLAG_MOTOR_RUNNING (1<<1)
#define FLAG_SYSTEM_ACTIVE (1<<2)


struct Settings {
    uint8_t motor_power;    // 0 = stop, 255 = full speed
    uint8_t motor_ontime;   // how long the motor runs [UNIT=0.1s]
    uint8_t motor_offtime;  // how long the motor stops [UNIT=0.1s]
    uint8_t time_left;      // time left until the motor runs/stops [UNIT=0.1s]
    uint8_t flags;          // use in combination with the 'FLAG_' definitions
};



void settings_load(struct Settings *psettings);
void settings_save(struct Settings *psettings);




#endif // SETTINGS_H
