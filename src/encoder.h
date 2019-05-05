#ifndef ENCODER_H
#define ENCODER_H

#include <avr/io.h>

void encoder_init();
int8_t encoder_read();
uint8_t encoder_switch();

#endif // ENCODER_H
