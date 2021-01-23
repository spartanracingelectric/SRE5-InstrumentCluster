/*
 * Instrument Cluster Test.c
 *
 * Created: 8/15/2020 11:58:41 AM
 * Author : wayne
 */ 

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "i2c.h"
#include "indicators.h"
#include "1602lcd.h"
#include "buttons.h"

int main(void) {
	i2c__init();
	LCD__init();
	buttons__init();
	LCD__wake();
	LCD__clr();
	LCD__default();
	sei(); //initialize interupts for buttons
	
	rpm__set(0b1111111111111111);
	left_rgb__set(BLACK);
	right_rgb__set(BLACK);
	left_indicator__set(1); //TURN OFF BLUE LED #1 NOTE THAT 1 = OFF. 0 = ON
	right_indicator__set(1); //TURN OFF BLUE LED #1 NOTE THAT 1 = OFF. 0 = ON
	
	while (1) 
    {
	//in this case, 1 = ON; 0 = OFF
	_delay_ms(300);
	rpm__set(0b0000000011111111);
	_delay_ms(300);
	rpm__set(0b1111111100000000);
	_delay_ms(300);

	//1 = OFF; 0 = ON 
	uint16_t status = 0b1111111111111111;

	for(int i = 0; i < 16; i++) {
		status &= ~(1<<i);
		rpm__set(status);
		_delay_ms(100);

	}
	
	for(int i = 16; i >= 0; i--) {
		status |= (1<<i);
		rpm__set(status);
		_delay_ms(100);
	}	
	
	_delay_ms(300);
	left_indicator__set(0);
	_delay_ms(300);
	right_indicator__set(0);
	_delay_ms(300);
	left_rgb__set(RED);
	_delay_ms(300);
	left_rgb__set(WHITE);
	_delay_ms(300);
	left_rgb__set(BLUE);
	_delay_ms(300);
	left_rgb__set(GREEN);
	_delay_ms(300);
	right_rgb__set(RED);
	_delay_ms(300);
	right_rgb__set(WHITE);
	_delay_ms(300);
	right_rgb__set(BLUE);
	_delay_ms(300);
	right_rgb__set(GREEN);
	_delay_ms(300);
	right_rgb__set(BLACK);
	_delay_ms(300);
	left_rgb__set(BLACK);
	_delay_ms(300);
	left_indicator__set(1);
	_delay_ms(300);
	right_indicator__set(1);
	_delay_ms(300);
	}
}