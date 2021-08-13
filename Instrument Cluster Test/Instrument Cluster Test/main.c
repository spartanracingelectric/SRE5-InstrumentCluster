/*
 * Instrument Cluster Test.c
 *
 * Created: 8/15/2020 11:58:41 AM
 * Author : wayne
 */ 

 #ifndef F_CPU
 #define F_CPU 20000000UL
 #endif

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "i2c.h"
#include "indicators.h"
#include "1602lcd.h"

//LCD ports
#define RS 0 //PCF P0
#define E  2 //P2
#define BL 3 //P3 goes to backlight A/K on LCD
#define D4 4 //P4
#define D5 5 //P5
#define D6 6 //P6
#define D7 7 //P7

const uint8_t LCD_RS = (1<<RS), //(1<<0)
LCD_E  = (1<<E),  //(1<<2)
LCD_BL = (1<<BL), //(1<<3)
LCD_D4 = (1<<D4), //(1<<4)
LCD_D5 = (1<<D5), //(1<<5)
LCD_D6 = (1<<D6), //(1<<6)
LCD_D7 = (1<<D7); //(1<<7)

int main(void) {
	i2c__init();
	timer__init();

	rpm__set(0b1111111111111111);
	left_rgb__set(BLACK);
	right_rgb__set(BLACK);
	left_indicator__set(1); //TURN OFF BLUE LED #1 NOTE THAT 1 = OFF. 0 = ON
	right_indicator__set(1); //TURN OFF BLUE LED #1 NOTE THAT 1 = OFF. 0 = ON

    while (1) 
    {
	_delay_ms(300);
	rpm__set(0b0000000011111111);
	_delay_ms(300);
	rpm__set(0b1111111100000000);
	_delay_ms(300);

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

	//test RGB and blue indicators 
	/*
	left_rgb__set(RED);
	right_rgb__set(RED);
	_delay_ms(500);
	left_rgb__set(BLACK);
	right_rgb__set(BLACK);
	_delay_ms(500);
	right_rgb__set(GREEN);
	left_rgb__set(GREEN);
	_delay_ms(500);
	left_rgb__set(BLACK);
	right_rgb__set(BLACK);
	_delay_ms(500);
	left_rgb__set(BLUE);
	right_rgb__set(BLUE);
	_delay_ms(500);
	left_rgb__set(BLACK);
	right_rgb__set(BLACK);
	_delay_ms(500);
	*/
	
	
	left_rgb__set(ORANGE);
	right_rgb__set(BLACK);
	_delay_ms(300);
	left_rgb__set(PINK);
	right_rgb__set(VIOLET);
	_delay_ms(300);
	left_rgb__set(WHITE);
	right_rgb__set(ORANGE);
	_delay_ms(300);
	left_rgb__set(GREEN);
	right_rgb__set(RED);
	_delay_ms(300);
	left_indicator__set(1); //TURN OFF BLUE LED #1 NOTE THAT 1 = OFF. 0 = ON
	right_indicator__set(0); //TURN OFF BLUE LED #1 NOTE THAT 1 = OFF. 0 = ON
	left_rgb__set(MAGENTA);
	right_rgb__set(CYAN);
	_delay_ms(300);
	left_rgb__set(CYAN);
	right_rgb__set(MAGENTA);
	_delay_ms(300);
	left_rgb__set(RED);
	right_rgb__set(GREEN);
	_delay_ms(300);
	left_rgb__set(ORANGE);
	right_rgb__set(VIOLET);
	_delay_ms(300);
	left_rgb__set(VIOLET);
	right_rgb__set(WHITE);
	_delay_ms(300);
	left_rgb__set(BLACK);
	right_rgb__set(ORANGE);
	_delay_ms(300);
	left_indicator__set(0);
	right_indicator__set(1); //TURN OFF BLUE LED #1 NOTE THAT 1 = OFF. 0 = ON
	_delay_ms(300);
	left_indicator__set(1);
	right_indicator__set(1);
	left_rgb__set(BLACK);
	right_rgb__set(BLACK);
	_delay_ms(300); 
	
	}
}
