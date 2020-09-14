#ifndef INDLED_H_
#define INDLED_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include "i2c.h"

///INDICATOR LED SECTIOn////////////////////////////////////////////////////////
//Indicator RGB LED #1 (ref by RGB1)
int Duty_Cyles1[3] = {100,100,100}; // {R, G, B}

//Indicator RGB LED #2 (ref by RGB2)
int Duty_Cyles2[3] = {100,100,100}; // {R, G, B}
	
//Indicator LED #3 (ref by LED1)
int LED1Flag = 0;
//Indicator LED #3 (ref by LED1)
int LED2Flag = 0;


//Variables used by timer and interrupt
volatile int timerTick =0;
volatile int bitstate = 0x7;


//Specifies color
enum RGB{R,G,B};

#define RGB1 0
#define RGB2 1
#define LED1 0
#define LED2 1

//Color Profiles
uint8_t RED[] = {
100, 0, 0};
uint8_t ORANGE[] = {
32, 2, 0};
uint8_t YELLOW[] = {
100, 100, 0};
uint8_t GREEN[] = {
0, 100, 0};
uint8_t BLUE[] = {
0, 0, 100};
uint8_t INDIGO[] = {
4, 0, 19};
uint8_t VIOLET[] = {
9, 0, 9};
uint8_t CYAN[] = {
0, 100, 100};
uint8_t MAGENTA[] = {
100, 0, 100};
uint8_t WHITE[] = {
100, 100, 100};
uint8_t BLACK[] = {
0, 0, 0};
uint8_t PINK[] = {
62, 2, 30};

//Interrupt that pulses at 100khz; 100 pulses = 100hz period

ISR(TIMER0_COMPA_vect)
{
	timerTick++; //Counts interrupt pulses
	if (timerTick >= 100) 
	{	
		timerTick = 0;
		bitstate =0b00000000;
		for (int i = 0; i<3; i++)
		{	
			if (Duty_Cyles1[i] == 0)
			{
				bitstate |= (1<<i);
			}
		}
		for (int i = 0; i<3; i++)
		{
			if (Duty_Cyles2[i] == 0)
			{
				bitstate |= (1<<(i+3));
			}
		}
		
		if (LED1Flag == 1)
		{
			bitstate |= (1<<6);
		}	
		if (LED2Flag == 1)
		{
			bitstate |= (1<<7);
		}
		twi_start(LED_INDICATORS);
		twi_write(bitstate); //TURN ON ALL COLORS
		twi_stop();
	}
	for (int i = 0; i<3; i++)
	{
		if (timerTick == Duty_Cyles1[i])
		{
			bitstate |= (1<<i);
			twi_start(LED_INDICATORS);
			twi_write(bitstate); //TURNS OFF SPECIFIED LED
			twi_stop();
		}
		if (timerTick == Duty_Cyles2[i])
		{
			bitstate |= (1<<(i+3)); //IMPORTANT REPLACE 5 WITH THE DISPLACEMENT OF RGB2 BITS FROM THE LEAST SIGNIFICANT BIT
			twi_start(LED_INDICATORS);
			twi_write(bitstate); //TURNS OFF SPECIFIED LED
			twi_stop();
		}
	}

}


//Timer initialization
void timer_Init()
{	
	// TIMER 0 for interrupt frequency 10000 Hz:
	cli(); // stop interrupts
	TCCR0A = 0; // set entire TCCR0A register to 0
	TCCR0B = 0; // same for TCCR0B
	TCNT0  = 0; // initialize counter value to 0
	// set compare match register for 10000 Hz increments
	OCR0A = 249; // = 20000000 / (8 * 10000) - 1 (must be <256)
	// turn on CTC mode
	TCCR0B |= (1 << WGM01);
	// Set CS02, CS01 and CS00 bits for 8 prescaler
	TCCR0B |= (0 << CS02) | (1 << CS01) | (0 << CS00);
	// enable timer compare interrupt
	TIMSK0 |= (1 << OCIE0A);
	sei(); // allow interrupts
}

//SETS LED COLOR via THE DUTY CYCLE
void rgb_set(int LED_ID, uint8_t color[])
{
	switch (LED_ID)
	{
		case RGB1:
		for (int i = 0; i < 3; i++)
		{
			Duty_Cyles1[i] = color[i];
		}
		break;
		
		case RGB2:
		for (int i = 0; i < 3; i++)
		{
			Duty_Cyles2[i] = color[i];
		}
		break;
	}
}

//RPM LED SECTION////////////////////////////////////////////////////////

void rpm_write(uint16_t LED_PATTERN)
{
	//1 = off; 0 = on
	uint8_t byte1 = (LED_PATTERN & 0xff), byte2 = (LED_PATTERN >>8);
	twi_start(LED_BAR_1);
	twi_write(byte1);
	twi_stop();
	
	twi_start(LED_BAR_2);
	twi_write(byte2);
	twi_stop();
}

void indicatorSet(int LED_ID, int status)
{
	switch (LED_ID)
	{
		case LED1:
		LED1Flag = status;
		break;
		
		case LED2:
		LED2Flag = status;
		break;
	}
	
}

#endif /* INDLED_H_ */