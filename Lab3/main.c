#define F_CPU 16000000UL //Unsigned long defining the 16MHz clock speed of the ATmega328p

#include <avr/io.h>
#include <util/delay.h>

//Pin definitions for the LCD screen, must be defined before lcd is imported
#define D4 eS_PORTD4
#define D5 eS_PORTD5
#define D6 eS_PORTD6
#define D7 eS_PORTD7
#define RS eS_PORTB0
#define EN eS_PORTB1

//Dummy leftover definitions for the 8 pin version
#define D0 eS_PORTD1
#define D1 eS_PORTD2
#define D2 eS_PORTD3
#define D3 eS_PORTD4

#include "lcd.h"

#define BAUD 9600 //Define baud rate, bits per second, serial monitor has too match
#define UBRR_VALUE  ((F_CPU / (16UL * BAUD)) - 1)   //USART Baud Rate Register = 103 at 16 MHz

#define LCD_COLS    16 //16 characters per column
#define MAX_CHARS   32 //32 characters max


void uart_init(void)
{
	//Baud register is 12 bit wide so need to split a high byte and low byte, shifting low byte to the right
    UBRR0H = (uint8_t)(UBRR_VALUE >> 8);
    UBRR0L = (uint8_t)UBRR_VALUE;
    UCSR0B = (1 << RXEN0) | (1 << TXEN0); //Reciever and transmitter bits set high
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);//Data format of 8 bits leaving no space for stop or parity
}

char uart_rx(void)
{
    while (!(UCSR0A & (1 << RXC0))); //Wait until recieve is complete,     
    return UDR0;
}

void uart_tx(char c)
{
    while (!(UCSR0A & (1 << UDRE0)));//Wait until buffer is empty 
    UDR0 = c;
}

void show_message(const char *buf, uint8_t len) //Pointer to message and const means no changing
{
    Lcd4_Clear(); //clearing the screen
    _delay_ms(2); //clear command takes 1.6ms
    Lcd4_Set_Cursor(1, 0);//Set cursor to the first character space

    for (uint8_t i = 0; i < len; i++) { //when characters reaches 16 go to next line
        if (i == LCD_COLS)
            Lcd4_Set_Cursor(2, 0);
        Lcd4_Write_Char(buf[i]);
    }
}

int main(void)
{
    char buf[MAX_CHARS]; //max characters
    uint8_t len = 0; //how many are on it rn

    DDRD |= 0xF0; // PD4-PD7 outputs (LCD data)
    DDRB |= (1 << DDB0) | (1 << DDB1); // PB0 = RS, PB1 = EN outputs 
	
	//setup device and display a waiting message
    uart_init();
    Lcd4_Init();
    Lcd4_Clear();
    _delay_ms(2);
    Lcd4_Set_Cursor(1, 0);
    Lcd4_Write_String("Waiting...");

    while (1) {
        char c = uart_rx(); //wait for one char

        if (c == '\r' || c == '\n') { //If it enter then display message
            if (len > 0) {
                show_message(buf, len);
                len = 0;
            }
        } else {
            buf[len++] = c;
            uart_tx(c); // echo back to the terminal

            if (len == MAX_CHARS) {  // screen full: show it anyway 
                show_message(buf, len);
                len = 0;
            }
        }
    }
}