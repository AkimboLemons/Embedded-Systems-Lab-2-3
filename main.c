#include <avr/io.h>
#include <stdint.h>
#include <stdlib.h>

//Declare methods
void dutyCycleCalculate(float);
void waitForButton(void);
void T1Delay1ms(void);
void delayMS(uint16_t ms);

//Avoid magic numbers
float dutyCycle1 = 0.00;
float dutyCycle2 = 25.00;
float dutyCycle3 = 62.50;
float dutyCycle4 = 87.50;

//Calculate duty cycle as based in Lab2 sheet but with added protection from /0
void dutyCycleCalculate(float n)
{
	if (n == 0) {
		OCR0A = 0x00;
	} 
	else if (n > 0) {
		OCR0A = (n * 256)/100;	
	}
	
}

//Standard button wait with debounce
void waitForButton(void) 
{
		while (PIND & (1 << PD2)) {}
		delayMS(20);
		
		while (!(PIND & (1 << PD2))) {}
		delayMS(20);
}

int main(void)
{	
	//Set data direction for DDRD, 6 as output, 2 as input
	DDRD |= (1 << PD6);
	DDRD &= ~(1 << PD2);
	PORTD |=  (1 << PD2); //Pullup pin
	
	TCCR0A = (1 << COM0A1) | (1 << WGM01) | (1 << WGM00);  // fast PWM, non-inverting OC0A
	TCCR0B = (1 << CS02) | (1 << CS00);                    // prescaler 1024 = 61.04 Hz             
	while(1) {	
		dutyCycleCalculate(dutyCycle1);
		
		waitForButton();
		dutyCycleCalculate(dutyCycle2);
		
		waitForButton();
		dutyCycleCalculate(dutyCycle3);
		
		waitForButton();
		dutyCycleCalculate(dutyCycle4);
		waitForButton();
	}
}

//Prescaler 64 is the 16Mhz ATMEga328p op speed 16mhz/64 = 250 kHz, 4microsecs per tick
//1ms = 1000microsecs / 4 microsecs = 250 ticks for 1ms
//Delay is 1ms meaning it can be called exactly for the required ms amount
void T1Delay1ms(void)
{
	TCNT1  = 65286;                         // 65536 - 65286 = 250 ticks x 4 us = 1 ms
	TCCR1A = 0x00;                          //normal Mode
	TCCR1B = (1 << CS11) | (1 << CS10);     // start, prescaler 64
	while (!(TIFR1 & (1 << TOV1))) { }      // wait for overflow
	TCCR1B = 0x00;                          // stop
	TIFR1  = (1 << TOV1);                   // clear flag
}

void delayMS(uint16_t ms)
{
	while (ms--) T1Delay1ms();
}