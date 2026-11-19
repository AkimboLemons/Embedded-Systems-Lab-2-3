
// LCD Display
#include "lcd.h"
#include <avr/io.h>
#include <stdint.h>
#include <stdlib.h>

#define	D4	eS_PORTD4
#define	D5	eS_PORTD5
#define	D6	eS_PORTD6
#define	D7	eS_PORTD7
#define	RS	eS_PORTC6
#define	EN	eS_PORTC7

//the folowing code has ben made from the library, we can probably use this to build out.
unsigned int all = 0xFF;    // save me from magic numbers.
unsigned int zero = 0x00;

void sendScroll(char msg);
void USART0_init (unsigned int ubrr);
unsigned char USART_Receive();
unsigned char* USARTReceiveString();


void main(void)
{
    DDRB = all;                        // Set PORTB as output
    DDRD = all;                         // Set PORTD as output
    DDRC = all;                         // Set PORTC as output

    USART0_init(103);                   // 9600 baud rate for 16MHz clock
    pinChange(eS_PORTB0,0);             // set the LCD pin to port B
    Lcd4_Init();                        // Initialize the LCD, from LCD.h
    Lcd4_Clear();                       // Clear the LCD
    Lcd4_Write_String("25 character max length");     // Display initialization message
    while(1) {
        if (UCSR0A & (1<<RXC0)) {
            //we want code here that can read from the usart and after say a delay, to know when the message is finished, write it to the LCD
            sendScroll(USARTReceiveString());
        }
 
    }   
}


char* USARTReceiveString(){        // this should be able to read the usart and join them into a string,

    unsigned char buffer[25];                  // buffer to hold the incoming string
    unsigned int i = 0;                        // index for the buffer
    unsigned int x = 0;                        // counter for the number of characters received

    while(1) {                        // loop until we have received the full message
        unsigned char c = USART_Receive();     // receive a character from the USART
        buffer[i++] = c;              // add the character to the buffer
        if (i == 25) {               //once the buffer is full end message
            break;                  // this assumes that the USART will send blank characters after the message is sent.
        }
    }
        return buffer;                    // return the completed string
}

void USART0_init (unsigned int ubrr) {
    // Set baud rate
    UBRR0H = (unsigned char)(ubrr>>8);
    UBRR0L = (unsigned char)ubrr;
    // disable double speed mode
    UCSR0A = (0<<U2X0); 
    // Enable receiver and transmitter
    UCSR0B = (1<<RXEN0)|(1<<TXEN0);
    // Set frame format: 8data, 1stop bit
    UCSR0C = (1<<UCSZ01)|(1<<UCSZ00);
}

unsigned char USART_Receive()
{
/* Wait for data to be received */
    while ( !(UCSR0A & (1<<RXC0)) );
    /* Get and return received data from buffer */
    return UDR0;
}

void sendScroll(char *msg) {
    Lcd4_Write_Char(*msg);             // Display the message
    Lcd4_Shift_Left();                  // Shift the display to the left
}

