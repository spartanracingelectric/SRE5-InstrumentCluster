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
#include "led.h"
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

int main(void)
{
    /* Replace with your application code */
    while (1) 
    {
	//test RPM Bar
	twi_init(); 
	timer_Init();
	rpm_write(0x0); //clear bar
	_delay_ms(500);
	rpm_write(0b1010101010101010); //turn on odd leds
	_delay_ms(500);
	rpm_write(0b0101010101010101); //turn on even leds
	_delay_ms(500);

	//test RGB LED Indicator
	for (int i =0; i<2; i++)
	{
		rgb_set(i, BLACK);
		_delay_ms(300);
		rgb_set(i, WHITE);
		_delay_ms(300);
		rgb_set(i, BLACK);
		_delay_ms(300);
		rgb_set(i, RED);
		_delay_ms(300);
		rgb_set(i, GREEN);
		_delay_ms(300);
		rgb_set(i, BLUE);
		_delay_ms(300);
		rgb_set(i, BLACK);
	}

	//test LED indicators
	twi_start(LED_INDICATORS);
	twi_write(0b00000001);
	_delay_ms(500);
	twi_write(0b00000011);
	_delay_ms(500);
	twi_write(0b00000010);
	_delay_ms(500);
	twi_write(0b00000000);

	/*
	//test LCD
	LCD_init(I2C_ADDRESS, LCD_RS, LCD_E, LCD_BL, LCD_D4, LCD_D5, LCD_D6, LCD_D7);	//Initialization of LCD, twi_init() is called here. ptr to the lcd port array is passed
	LCD_wake();																		//Wake function to test LCD functionality
	LCD_str("Hello");
	LCD_cmd(0xC0);
	LCD_str("AHHHHH");
	_delay_ms(500);
	LCD_clr();
    */ 
	}
}

