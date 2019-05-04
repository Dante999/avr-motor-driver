#ifndef MOTOR_H_
#define MOTOR_H_

#include <avr/io.h>

void motor_init();
void motor_cycle();
void motor_power(uint8_t power);
void motor_frequency(uint16_t frequency);

#endif
