#include <avr/io.h>


#define PIXEL_WIDTH  128
#define PIXEL_HEIGHT  64


#define X_MAX   PIXEL_WIDTH
#define Y_MAX   PIXEL_HEIGHT/8

static uint8_t g_graphx_buffer[X_MAX][Y_MAX] = {0x00};


void graphx_set_pixel(uint8_t x, uint8_t y) {

    uint8_t y_byte = y/8;
    uint8_t y_bit  = y%8;


    g_graphx_buffer[x][y_byte] |= (1<<y_bit);
}

uint8_t graphx_get_pixel(uint8_t x, uint8_t y) {
    uint8_t y_byte = y/8;
    uint8_t y_bit  = y%8;


    if( g_graphx_buffer[x][y_byte] & (1<<y_bit) ) {
        return 1;
    }
    else {
        return 0;
    }
}

uint8_t *graphx_get_buffer() {
    return (uint8_t*) g_graphx_buffer;
}


