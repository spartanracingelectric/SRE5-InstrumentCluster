/* LCD COMMANDS http://www.dinceraydin.com/lcd/commands.htm
 * HD44780 DATASHEET https://www.makerguides.com/wp-content/uploads/2019/02/HD44780-Datasheet.pdf
 * PCF8574 DATASHEET https://www.makerguides.com/wp-content/uploads/2019/02/PCF8574-Datasheet.pdf
 * PCF8574T to HD44780 PINOUT https://www.playembedded.org/blog/hd44780-backpack-stm32/
 */

#define F_CPU 16000000UL

#include <avr/io.h> //Included in 1602lcd.h
#include <util/delay.h> //Included in 1602lcd.h
#include "i2c.h"
#include "1602lcd.h"
#include <util/twi.h> //Included in i2c.h

//#define LCD_DDR DDRD //direction
//#define LCD_PORT PORTD
#define RS 0 //PCF P0
#define E 2 //P2
#define BL 3 //P3 goes to backlight A/K on LCD
#define D4 4 //P4
#define D5 5 //P5
#define D6 6 //P6
#define D7 7 //P7
#define I2C_ADDRESS 0x27 //PCF8574T

const uint8_t LCD_RS = (1<<RS), //(1<<0)
		LCD_E  = (1<<E),  //(1<<2)
		LCD_BL = (1<<BL), //(1<<3)
		LCD_D4 = (1<<D4), //(1<<4)
		LCD_D5 = (1<<D5), //(1<<5)
		LCD_D6 = (1<<D6), //(1<<6)
		LCD_D7 = (1<<D7); //(1<<7)

//const uint8_t LCDPORTS[7] = {LCD_RS, LCD_E, LCD_BL, LCD_D4, LCD_D5, LCD_D6, LCD_D7};
//const uint8_t *LCDPORTS_ptr = &LCDPORTS[0];

int main(void)
{
	LCD_init(I2C_ADDRESS, LCD_RS, LCD_E, LCD_BL, LCD_D4, LCD_D5, LCD_D6, LCD_D7);			/* Initialization of LCD, twi_init() is called here. ptr to the lcd port array is passed */
    LCD_wake();			/* Wake function to test LCD functionality */
	LCD_str("Made by Vincent");
	LCD_cmd(0xC0);
	LCD_str("And Nick!");
	_delay_ms(500);
	LCD_clr();
	while(1);
}

