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
	twi_init();
	timer_Init();
	
	//test LCD	
	LCD_init(LCD_ADDRESS, LCD_RS, LCD_E, LCD_BL, LCD_D4, LCD_D5, LCD_D6, LCD_D7);	//Initialization of LCD, twi_init() is called here. ptr to the lcd port array is passed
	LCD_wake();																		//Wake function to test LCD functionality
	LCD_str("Hello");
	LCD_cmd(0xC0);
	LCD_str("AHHHHH");
	_delay_ms(500);
	LCD_clr();

    while (1) 
    {
	rpm_write(0b1111111111111111); //clear bar
	indicatorSet(LED1, 1);
	indicatorSet(LED2, 1); //TURN OFF BLUE LED #1 NOTE THAT 1 = OFF. 0 = ON
	rgb_set(0, BLACK);
	rgb_set(1, BLACK);

	//test RPM Bar
	/*
	rpm_write(0b1111111111111111); //clear bar
	_delay_ms(500);
	rpm_write(0b1111111100000000); //turn on right
	_delay_ms(500);
	rpm_write(0b0000000011111111); //turn on left
	_delay_ms(500);
	rpm_write(0b0000000000000000); //turn on all
	_delay_ms(500);
	rpm_write(0b1111111111111111); //clear bar
	_delay_ms(500);
	
	//test RGB and blue indicators 
	rgb_set(0, ORANGE);
	rgb_set(1, BLACK);
	_delay_ms(300);
	rgb_set(0, PINK);
	rgb_set(1, VIOLET);
	_delay_ms(300);
	rgb_set(0, WHITE);
	rgb_set(1, ORANGE);
	_delay_ms(300);
	rgb_set(0, GREEN);
	rgb_set(1, RED);
	_delay_ms(300);
	indicatorSet(LED1, 1); //TURN OFF BLUE LED #1 NOTE THAT 1 = OFF. 0 = ON
	indicatorSet(LED2, 0); //TURN OFF BLUE LED #1 NOTE THAT 1 = OFF. 0 = ON
	rgb_set(0, MAGENTA);
	rgb_set(1, CYAN);
	_delay_ms(300);
	rgb_set(0, CYAN);
	rgb_set(1, MAGENTA);
	_delay_ms(300);
	rgb_set(0, RED);
	rgb_set(1, GREEN);
	_delay_ms(300);
	rgb_set(0, ORANGE);
	rgb_set(1, VIOLET);
	_delay_ms(300);
	rgb_set(0, VIOLET);
	rgb_set(1, WHITE);
	_delay_ms(300);
	rgb_set(0, BLACK);
	rgb_set(1, ORANGE);
	_delay_ms(300);
	indicatorSet(LED1, 0);
	indicatorSet(LED2, 1); //TURN OFF BLUE LED #1 NOTE THAT 1 = OFF. 0 = ON
	_delay_ms(300);
	indicatorSet(LED1, 1);
	indicatorSet(LED2, 1);
	rgb_set(0, BLACK);
	rgb_set(1, BLACK);
	_delay_ms(300);
	*/
	

    
	}
}

