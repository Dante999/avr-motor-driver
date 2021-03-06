/*******************************************************************************
 *
 * File    : uart.c
 *
 * Author  : Dante999
 * Date    : 28.12.2018
 *
 * Tabsize : 4
 * License : GNU GPL v2
 *
 * writes to the uart serial interface
 *
 *
 ******************************************************************************/
#include "uart.h"
#include <stdio.h>
#include <avr/interrupt.h>
#include <string.h>


#ifndef F_CPU
    #error F_CPU is not defined!!
#endif





#define CR   "\r"
#define CRLF "\r\n"
#define NEWLINE CR


volatile uint8_t g_buffer_complete = 0;     // 1 .. String komplett empfangen
volatile uint8_t g_buffer_index = 0;
volatile char g_buffer[UART_BUFFER_SIZE] = "";





#define BAUD 9600UL      // Target Baudrate

// Calculations (do not change)
#define UBRR_VAL    ((F_CPU+BAUD*8)/(BAUD*16)-1)    // Value for the UBRR Register
#define BAUD_REAL   (F_CPU/(16*(UBRR_VAL+1)))       // Real Baudrate
#define BAUD_ERROR  ((BAUD_REAL*1000)/BAUD)         // Error Rate in promille, 1000 = 0K

#if ((BAUD_ERROR<990) || (BAUD_ERROR>1010))
    #error Rate of Baudrate Errors greater than 1%! Change system-clock or baudrate!
#endif



/*******************************************************************************
 * @brief initializes the uart interface
 *
 * @param  none
 *
 * @return none
 ******************************************************************************/
void uart_init() {
    UBRR0H = UBRR_VAL >> 8;
    UBRR0L = UBRR_VAL & 0xFF;

    UCSR0B |= (1<<TXEN0) | (1<<RXEN0) | (1<<RXCIE0);               // enable UART TX
    UCSR0C = (1<<UCSZ01)|(1<<UCSZ00);   // Asynchron (8-bit)(no-parity)(1-stop-bit)
}


/*******************************************************************************
 * @brief prints single char to the uart interface
 *
 * @param  c     the char to print
 *
 * @return none
 ******************************************************************************/
void uart_putc(char c) {

    while (!(UCSR0A & (1<<UDRE0)))
    {
        // waiting until sender is free...
    }

    UDR0 = c;                                    // save character for sending
}


/*******************************************************************************
 * @brief prints an 8-bit integer to the uart interface
 *
 * @param  i     the integer to print
 *
 * @return none
 ******************************************************************************/
void uart_puti(uint8_t i) {

    char buffer[4];

    sprintf(buffer, "%d", i);

    uart_puts(buffer);
}


/*******************************************************************************
 * @brief prints a string to the uart interface (without any newline)
 *
 * @param  *s     pointer to the string which should be printed
 *
 * @return none
 ******************************************************************************/
void uart_puts(char *s) {

    while(*s != '\0') {
        uart_putc(*s);
        s++;
    }
}


/*******************************************************************************
 * @brief prints a string to the uart interface (with a newline)
 *
 * @param  *s     pointer to the string which should be printed
 *
 * @return none
 ******************************************************************************/
void uart_putsln(char *s) {
    uart_puts(s);
    uart_puts(NEWLINE);
}




static void handle_received_char(unsigned char c) {

    if( c == '\b' && g_buffer_index > 0 ) {
        g_buffer_index--;
        uart_puts("\b \b");
    }
    else if( c != '\r' &&
             c != '\n' &&
             g_buffer_index < UART_BUFFER_SIZE-1 ) {

        g_buffer[g_buffer_index] = c;
        g_buffer_index++;

        uart_putc(c);
    }
    else {
        g_buffer[g_buffer_index] = '\0';
        g_buffer_index = 0;
        g_buffer_complete = 1;

        uart_putsln("");
    }

}

uint8_t uart_is_complete() {
    return g_buffer_complete;
}

void uart_read_buffer(char *buffer) {
    memcpy(buffer, (void*)g_buffer, UART_BUFFER_SIZE);

    g_buffer_complete = 0;
}

ISR(USART_RX_vect)
{

    unsigned char c = UDR0;

    if(!g_buffer_complete) {
        handle_received_char(c);
    }

}






