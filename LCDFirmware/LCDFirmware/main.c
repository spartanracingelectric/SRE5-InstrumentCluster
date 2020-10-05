/* LCD COMMANDS http://www.dinceraydin.com/lcd/commands.htm
 * HD44780 DATASHEET https://www.makerguides.com/wp-content/uploads/2019/02/HD44780-Datasheet.pdf
 * PCF8574 DATASHEET https://www.makerguides.com/wp-content/uploads/2019/02/PCF8574-Datasheet.pdf
 * PCF8574T to HD44780 PINOUT https://www.playembedded.org/blog/hd44780-backpack-stm32/
 */

#define F_CPU 16000000UL

#include <avr/io.h> //Included in 1602lcd.h
#include <avr/interrupt.h>
#include <util/delay.h> //Included in 1602lcd.h
#include "i2c.h"
#include "1602lcd.h"
#include "buttons.h"

//#define LCD_DDR DDRD //direction
//#define LCD_PORT PORTD

/*
const uint8_t LCD_RS = (1<<RS), //(1<<0)
		LCD_E  = (1<<E),  //(1<<2)
		LCD_BL = (1<<BL), //(1<<3)
		LCD_D4 = (1<<D4), //(1<<4)
		LCD_D5 = (1<<D5), //(1<<5)
		LCD_D6 = (1<<D6), //(1<<6)
		LCD_D7 = (1<<D7); //(1<<7)
*/

//const uint8_t LCDPORTS[7] = {LCD_RS, LCD_E, LCD_BL, LCD_D4, LCD_D5, LCD_D6, LCD_D7};
//const uint8_t *LCDPORTS_ptr = &LCDPORTS[0];

int main(void)
{
	//cli();
	LCD_init();			/* Initialization of LCD, twi_init() is called here. ptr to the lcd port array is passed */
	buttons_init();
    LCD_wake();			/* Wake function to test LCD functionality */
	
	/**	
	PCMSK1 |= PCINT_BUT1 | PCINT_BUT2 | PCINT_BUT3 | PCINT_BUT4; //Enable pin change interrupts on pc0 -> pc3 pins ( Pin Change Mask Register 1 )
	PCICR |= (1<<PCIE1);  //When the PCIE1 bit is set (one) and the I-bit in the status register (SREG) is set (one), pin change interrupt 1 is enabled ( Pin Change Interrupt Control Register )
	**/
	/**
	TCCR0A = (1<<WGM01); //Set CTC bit
	OCR0A = 156; //Timer ticks per .01s, 16MHz Clk/1024
	TIMSK0 = (1 << OCIE0A);
	TCCR0B = (1<<CS02) | (1<<CS00); //Set Clk/1024
	**/
	
	LCD_default();
	
	sei(); //Enables interrupts by setting the global interrupt mask, must go at end of initialization phase after LCD_default()

	
	while (1) {
		
	}
}
/**
ISR(TIMER0_COMPA_vect) {
	
	if (state == 2) {
		extraTime++;
		LCD_cmd(0x87);
		LCD_int((extraTime/100)+1);
		if (extraTime > 600) { //600*.01s = 6s
			LCD_default();
			extraTime = 0;
		}
	}
}
**/