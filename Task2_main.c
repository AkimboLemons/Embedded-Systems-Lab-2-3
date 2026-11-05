#include <avr/io.h>
#include <stdint.h>

int main(void)
{
//Pin configuration
DDRD |= (1 << PD6);
DDRD |= (1 << PD5);

// ADC: AVcc ref, ADC0 (MUX = 0000), left-adjusted ? read ADCH
ADMUX  = (1 << REFS0) | (1 << ADLAR);

// ADCSRB = 0 (reset value) ? free-running trigger source

// Enable, auto-trigger, start first conversion, prescaler 128
ADCSRA = (1 << ADEN) | (1 << ADATE) | (1 << ADSC)
| (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);

//Timer0 Config
TCCR0A = (1 << COM0A1) | (1 << WGM01) | (1 << WGM00) | (1 << COM0B1) | (1 << COM0B0);  // fast PWM, non-inverting OC0A, inverting OC0B
TCCR0B = (1 << CS01); // prescaler 8

while (1) {
uint8_t duty = ADCH;
OCR0A = duty;
OCR0B = duty;
}
}
}
