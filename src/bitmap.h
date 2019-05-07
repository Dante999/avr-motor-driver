#ifndef BITMAP_H
#define BITMAP_H


#define BITMAP_WIDTH 5


#define BITMAP_CYLINDER 1
#define BITMAP_CONNECTING_ROD 1
#define BITMAP_CRANKSHAFT 2


#if BITMAP_CYLINDER == 1
static const uint8_t bitmap_cylinder[] = {
    0b01111110, // start top position
    0b00111110,
    0b11111110,
    0b00111110,
    0b01111110,
    0b11111000, // start bottom position
    0b11111000,
    0b11111000,
    0b11111000,
    0b11111000,
};
#elif BITMAP_CYLINDER == 2
static const uint8_t bitmap_cylinder[] = {
    0b01111110, // start top position
    0b00101010,
    0b11001010,
    0b00101010,
    0b01111110,
    0b11111000, // start bottom position
    0b10101000,
    0b00101000,
    0b10101000,
    0b11111000,
};
#endif

#if BITMAP_CONNECTING_ROD == 1
static const uint8_t bitmap_connecting_rod[] = {
    0b01111100, // start top position
    0b00001000,
    0b00001111,
    0b00001000,
    0b01111100,
    0b01111101, // start bottom position
    0b00100000,
    0b00111111,
    0b00100000,
    0b01111101
};
#elif BITMAP_CONNECTING_ROD == 2
static const uint8_t bitmap_connecting_rod[] = {
    0b01111100, // start top position
    0b00000000,
    0b00001111,
    0b00000000,
    0b01111100,
    0b01111101, // start bottom position
    0b00000000,
    0b00111111,
    0b00000000,
    0b01111101
};
#endif

#if BITMAP_CRANKSHAFT == 1
static const uint8_t bitmap_crankshaft[] = {
    0b01111100,
    0b00111000,
    0b00010000,
    0b00111000,
    0b01111100,
};
#elif BITMAP_CRANKSHAFT == 2
static const uint8_t bitmap_crankshaft[] = {
    0b00010000,
    0b00010000,
    0b00010000,
    0b00010000,
    0b00010000
};
#endif


#endif // BITMAP_H
