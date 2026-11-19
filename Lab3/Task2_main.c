
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

void main(void)
{
    DDRC = 0xFF;                        // Set PORTC as output
    DDRD = 0xFF;                        // Set PORTD as output

    Lcd4_Init();                        // Initialize the LCD, from LCD.h
    Lcd4_Clear();                       // Clear the LCD
    Lcd4_Write_String("init done");     // Display initialization message
    while(1) {

    }
}
