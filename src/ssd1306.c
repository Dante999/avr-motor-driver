/*******************************************************************************
 * @file ssd1306.c
 *
 * author               : dante999
 *
 * brief                : lightweight library for the ssd1306 ole-lcd display
 *
 * description          : this is a very-very small and lightweight library for
 *                        the I2C oled ssd1306.
 *
 * supported devices    : this is depending in the used I2C (TWI) library.
 *                        Currently this libarary is based on all avr devices
 *                        which are using the USI module as a TWI Master
 *
 * usage                :
 *
 *
*******************************************************************************/
#include "ssd1306.h"
#include "i2cmaster.h"
#include <avr/io.h>
#include <string.h>
#include "font_5x7.h"

/*******************************************************************************
 * settings for the ssd1306 display
*******************************************************************************/

// the I2C address of the display
#define SSD1306_ADDR        0x78

// the width of the lcd in pixels
#define SSD1306_LCDWIDTH    128

// the height of the lcd in pixels
#define SSD1306_LCDHEIGHT   64

// this is complete size of the lcd-buffer
#define LCD_BUFFER_SIZE     (SSD1306_LCDWIDTH*SSD1306_LCDHEIGHT/8)

// the size of the i2c-data packages (default: 8bytes will be send at once)
#define BLOCK_SIZE          8


/*******************************************************************************
 * ssd1306 specific i2c commands
 *
 * You should not change any of these!
*******************************************************************************/
#define SSD1306_SETCONTRAST         0x81
#define SSD1306_DISPLAYALLON_RESUME 0xA4
#define SSD1306_DISPLAYALLON        0xA5
#define SSD1306_NORMALDISPLAY       0xA6
#define SSD1306_INVERTDISPLAY       0xA7
#define SSD1306_DISPLAYOFF          0xAE
#define SSD1306_DISPLAYON           0xAF
#define SSD1306_SETDISPLAYOFFSET    0xD3
#define SSD1306_SETCOMPINS          0xDA
#define SSD1306_SETVCOMDETECT       0xDB
#define SSD1306_SETDISPLAYCLOCKDIV  0xD5
#define SSD1306_SETPRECHARGE        0xD9
#define SSD1306_SETMULTIPLEX        0xA8
#define SSD1306_SETLOWCOLUMN        0x00
#define SSD1306_SETHIGHCOLUMN       0x10
#define SSD1306_SETSTARTLINE        0x40
#define SSD1306_MEMORYMODE          0x20
#define SSD1306_COLUMNADDR          0x21
#define SSD1306_PAGEADDR            0x22
#define SSD1306_COMSCANINC          0xC0
#define SSD1306_COMSCANDEC          0xC8
#define SSD1306_SEGREMAP            0xA0
#define SSD1306_CHARGEPUMP          0x8D
#define SSD1306_EXTERNALVCC         0x1
#define SSD1306_SWITCHCAPVCC        0x2

// Scrolling #defines
#define SSD1306_ACTIVATE_SCROLL                         0x2F
#define SSD1306_DEACTIVATE_SCROLL                       0x2E
#define SSD1306_SET_VERTICAL_SCROLL_AREA                0xA3
#define SSD1306_RIGHT_HORIZONTAL_SCROLL                 0x26
#define SSD1306_LEFT_HORIZONTAL_SCROLL                  0x27
#define SSD1306_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL    0x29
#define SSD1306_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL     0x2A



static void ssd1306_command(uint8_t cmd);
//static void ssd1306_data(uint8_t data);
static void ssd1306_data(uint8_t *data, uint8_t size);



/*******************************************************************************
 * @brief   initialize the display
 *
 * @param   none
 *
 * @return  none
*******************************************************************************/
void ssd1306_init() {

    i2c_init();

    ssd1306_command(SSD1306_DISPLAYOFF);                // 0xAE

    ssd1306_command(SSD1306_SETDISPLAYCLOCKDIV);        // 0xD5
    ssd1306_command(0x80);                              // the suggested ratio 0x80

    ssd1306_command(SSD1306_SETMULTIPLEX);              // 0xA8

    ssd1306_command(64-1);

    ssd1306_command(SSD1306_SETDISPLAYOFFSET);          // 0xD3
    ssd1306_command(0x0);                               // no offset

    ssd1306_command(SSD1306_SETSTARTLINE);// | 0x0);    // line #0

    ssd1306_command(SSD1306_CHARGEPUMP);                // 0x8D
    ssd1306_command(0x14);                              // using internal VCC

    ssd1306_command(SSD1306_MEMORYMODE);                // 0x20
    ssd1306_command(0x00);                              // 0x00 horizontal addressing


    ssd1306_command(SSD1306_SEGREMAP | 0x1);            // rotate screen 180

    ssd1306_command(SSD1306_COMSCANDEC);                // rotate screen 180

    ssd1306_command(SSD1306_SETCOMPINS);                // 0xDA
    ssd1306_command(0x12);

    ssd1306_command(SSD1306_SETCONTRAST);               // 0x81
    ssd1306_command(0xCF);

    ssd1306_command(SSD1306_SETPRECHARGE);              // 0xd9
    ssd1306_command(0xF1);

    ssd1306_command(SSD1306_SETVCOMDETECT);             // 0xDB
    ssd1306_command(0x40);

    ssd1306_command(SSD1306_DISPLAYALLON_RESUME);       // 0xA4

    ssd1306_command(SSD1306_NORMALDISPLAY);             // 0xA6

    ssd1306_command(SSD1306_DEACTIVATE_SCROLL);

    ssd1306_command(SSD1306_DISPLAYON);                 //switch on OLED

    ssd1306_fill(0x00);                                 // clear the whole display



}


/******************************************************************************
 * @brief   fills the whole display with the given symbol
 *
 * @param   symbol      the symbol which should fill the display (0x00 = off)
 *
 * @return  none
******************************************************************************/
void ssd1306_fill(uint8_t symbol) {
    unsigned char buffer[BLOCK_SIZE];
    memset(buffer, symbol, sizeof(buffer));

    ssd1306_command(SSD1306_COLUMNADDR);
    ssd1306_command(0);
    ssd1306_command(SSD1306_LCDWIDTH-1);

    ssd1306_command(SSD1306_PAGEADDR);
    ssd1306_command(0);
    ssd1306_command(7);

   for( uint16_t i=0; i <= LCD_BUFFER_SIZE; i = i+BLOCK_SIZE) {
        ssd1306_data(buffer, BLOCK_SIZE);
   }
}


/******************************************************************************
 * @brief   sends the given command-byte to the display
 *
 * @param   cmd     the command-byte to send
 *
 * @return  none
******************************************************************************/
static void ssd1306_command(uint8_t cmd) {
    i2c_start_wait(SSD1306_ADDR+I2C_WRITE);     // set device address and write mode
    i2c_write(0x00);                            // following byte is a command
    i2c_write(cmd);                             // write command
    i2c_stop();                                 // set stop conditon = release bus
}


///******************************************************************************
// * @brief   sends the given  data-byte to the display
// *
// * @param   data    the data-byte to send
// *
// * @return  none
//******************************************************************************/
//static void ssd1306_data(uint8_t data) {
//    i2c_start_wait(SSD1306_ADDR+I2C_WRITE);     // set device address and write mode
//    i2c_write(0x40);                            // following byte is data
//    i2c_write(data);                             // write data
//    i2c_stop();                                 // set stop conditon = release bus
//}


/******************************************************************************
 * @brief   sends multiple data-bytes at once
 *
 * @param   *data   a pointer to the data-streama

 * @param   size    the size of data-bytes (length)
 *
 * @return  none
******************************************************************************/
static void ssd1306_data(uint8_t *data, uint8_t size) {

    i2c_start_wait(SSD1306_ADDR+I2C_WRITE);     // set device address and write mode
    i2c_write(0x40);                            // following byte is data

    for(uint8_t i=0; i<size; i++) {
        i2c_write(data[i]);                             // write data
    }

    i2c_stop();                                 // set stop conditon = release bus

}


/******************************************************************************
 * @brief   draws the given tile to the display
 *
 * @param   column  the column where the tile should be placed (0 to 127)
 *
 * @param   page    the page where the tile should be placed (0 to 7)
 *                  where 0 is on the top and 7 on the bottom
 *
 * @param   *tile   the tile which should be displayed (8x8 bits - uint8_t[8])
 *
 * @return  none
******************************************************************************/
void ssd1306_draw_tile_colpage(uint8_t column, uint8_t page, uint8_t *tile) {

    ssd1306_command(SSD1306_COLUMNADDR);
    ssd1306_command(column);
    ssd1306_command(column+7);

    ssd1306_command(SSD1306_PAGEADDR);
    ssd1306_command(page);
    ssd1306_command(page);

    ssd1306_data(tile, 8);
}


uint8_t ssd1306_putc(uint8_t column, uint8_t page, char c) {

    uint16_t letter_index = (c-' ')*FONT_WIDTH;

    const uint8_t *ptile = (const uint8_t*) (font+letter_index);

    return ssd1306_draw_bitmap(column, page, ptile, FONT_WIDTH);
}


uint8_t ssd1306_puts(uint8_t column, uint8_t page, char *s) {

    while(*s != '\0') {
        column = ssd1306_putc(column, page, *s);
        s++;
    }

    return column;
}

uint8_t ssd1306_draw_bitmap(uint8_t column, uint8_t page, const uint8_t *ptile, uint8_t width) {
    ssd1306_command(SSD1306_COLUMNADDR);
    ssd1306_command(column);
    ssd1306_command(column+width-1);

    ssd1306_command(SSD1306_PAGEADDR);
    ssd1306_command(page);
    ssd1306_command(page);

    ssd1306_data((uint8_t*) ptile, width);

    return column+width+1;
}

/******************************************************************************
 * @brief   draws the given tile to the display
 *
 * @param   x       the x-coordinate (0 -> left)
 *
 * @param   y       the y-coordinate (0 -> top)
 *
 * @param   tile    the tile which should be displayed (8x8 bits - uint8_t[8]
 *
******************************************************************************/
void ssd1306_draw_tile_xy(uint8_t x, uint8_t y, uint8_t *tile) {

    uint8_t start_page  = y/8;                  // find out on which page the tile starts
    uint8_t offset      = y%8;                  // find out the offset
    uint8_t end_page    = (y+7)/8;              // find out on which page the tile ends

    // if this is true, the tile fits on one page
    if( offset == 0) {
        ssd1306_draw_tile_colpage(x, start_page, tile);
    }
    // crap, we have to split the tile on two pages
    else {
        uint8_t start_tile[8];
        uint8_t end_tile[8];

        for(uint8_t i=0; i<8; i++) {
            start_tile[i] = tile[i] << offset;
            end_tile[i]   = tile[i] >> (8-offset);
        }

        ssd1306_draw_tile_colpage(x, start_page, start_tile);
        ssd1306_draw_tile_colpage(x, end_page,   end_tile);
    }

}


/*******************************************************************************
 * @brief   draws the complete given buffer to the lcd
 *
 * the size of the buffer must be equal with the defined 'LCD_BUFFER_SIZE'.
 *
 * @param   *buffer     a pointer to the allocated buffer with the content
 *
 * @return  none
*******************************************************************************/
void ssd1306_draw_buffer(uint8_t *buffer) {

    ssd1306_command(SSD1306_COLUMNADDR);
    ssd1306_command(0);
    ssd1306_command(SSD1306_LCDWIDTH-1);

    ssd1306_command(SSD1306_PAGEADDR);
    ssd1306_command(0);
    ssd1306_command(7);

   for( uint16_t i=0; i <= LCD_BUFFER_SIZE; i = i+BLOCK_SIZE) {
        ssd1306_data(buffer, BLOCK_SIZE);
   }

}
