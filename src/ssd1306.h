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
void ssd1306_draw_tile_xy(uint8_t x, uint8_t y, uint8_t *tile);
void ssd1306_init();
void ssd1306_fill(uint8_t symbol);
void ssd1306_draw_tile_colpage(uint8_t column, uint8_t page, uint8_t *tile);
void ssd1306_command(uint8_t cmd);
void ssd1306_data(uint8_t data);
void ssd1306_data_block(uint8_t *data, uint8_t size);

#endif
