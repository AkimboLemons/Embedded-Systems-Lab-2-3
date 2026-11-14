#include <avr/io.h>      // Enables register names like DDRB and PORTB
#include <util/delay.h>  // Enables delay functions like _delay_ms

int main(void) {
    // Set PB5 (Arduino Pin 13 / built-in LED) as an output
    DDRB |= (1 << PB5); 

    while (1) {
        PORTB |= (1 << PB5);   // Turn the LED ON (High)
        _delay_ms(250);       // Wait for 1 second

        PORTB &= ~(1 << PB5);  // Turn the LED OFF (Low)
        _delay_ms(250);       // Wait for 1 second
    }

    return 0;
}
