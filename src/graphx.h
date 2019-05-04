#ifndef GRAPHX_H
#define GRAPHX_H

#include <avr/io.h>


void graphx_set_pixel(uint8_t x, uint8_t y);
uint8_t graphx_get_pixel(uint8_t x, uint8_t y);
uint8_t *graphx_get_buffer();


#endif // GRAPHX_H
