#include <avr/io.h>
#include <stdint.h>

int main(void)
{
	//Pin configuration
	DDRB |= (1 << PB1);
	
	// ADC: AVcc ref, ADC0 (MUX = 0000)
	ADMUX  = (1 << REFS0);

	// Enable, auto-trigger, start first conversion, prescaler 128
	ADCSRA = (1 << ADEN) | (1 << ADATE) | (1 << ADSC)
	| (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
	
	ICR1 = 39999;		//input capture register for timer 1, 39999 corasponds to 50Hz, this frees up OCR1A for PWM output
	OCR1A = 3000;		//output capture register A for timer 1, used to generate the PWM signal
	
	// Timer1: mode 14 (fast PWM, TOP = ICR1), non-inverting OC1A, prescaler 8
	TCCR1A = (1 << COM1A1) | (1 << WGM11);
	TCCR1B = (1 << WGM13) | (1 << WGM12) | (1 << CS11);	
	
	while (1) {
		OCR1A = 2000 + ((uint32_t)ADC * 2000) / 1023;			//to convert from desired angle to corasponding duty cycle
	}
}

