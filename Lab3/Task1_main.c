//Serial communication

#include <avr/io.h>
#include <stdint.h>
#include <stdlib.h>

void main(){

    PRR = (0 << PRUSART0);
    DDRB = 0xFF; // Set PORTB as output
    PORTB = 0x00; // Initialize PORTB to 0
    USART0_init(103); // 9600 baud rate for 16MHz clock
    while(1){
        LED(USART_Receive());
    }
    
}

void LED(unsigned char data){
  
    if (data == 'Y') {
        PORTB = 0xFF; // Turn on all LEDs
    } else if (data == 'N') {
        PORTB = 0x00; // Turn off all LEDs
    }
}

void USART0_init (unsigned int ubrr) {
    // Set baud rate
    UBRR0H = (unsigned char)(ubrr>>8);
    UBRR0L = (unsigned char)ubrr;
    // Enable receiver and transmitter
    UCSR0B = (1<<RXEN0)|(1<<TXEN0);
    // Set frame format: 8data, 1stop bit
    UCSR0C = (1<<UCSZ01)|(1<<UCSZ00);
}

unsigned char USART_Receive( void )
{
/* Wait for data to be received */
    while ( !(UCSR0A & (1<<RXC0)) );
    /* Get and return received data from buffer */
    return UDR0;
}