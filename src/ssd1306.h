/*******************************************************************************
 * @file ssd1306.h
 *
 * author               : dante999
 *
 * see the source-file for further documentation
 *
*******************************************************************************/
#ifndef _SSD1306_H
#define _SSD1306_H

#include <avr/io.h>



/*******************************************************************************
 * function prototypes
*******************************************************************************/
void ssd1306_init();
uint8_t ssd1306_putc(uint8_t column, uint8_t page, char c);
uint8_t ssd1306_puts(uint8_t column, uint8_t page, char *s);



void ssd1306_draw_pagetile(uint8_t column, uint8_t page, const uint8_t  *tile, uint8_t tile_width);


void ssd1306_draw_tile_xy(uint8_t x, uint8_t y, uint8_t *tile);
void ssd1306_draw_buffer(uint8_t *buffer);

void ssd1306_fill(uint8_t symbol);
void ssd1306_draw_tile_colpage(uint8_t column, uint8_t page, uint8_t *tile);

#endif
