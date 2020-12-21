#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include "spi.h"
#include "1602lcd.h"
#include "i2c.h"
#include "diskio.h"
#include "pff.h"

#define ACK 0x7E

/* ARDUINO PRO MINI PINOUT:
 * CS:   PB2 (pin 10)
 * SCK:  PB5 (pin 13)
 * MOSI: PB3 (pin 11)
 * MISO: PB4 (pin 12)
 */

int main(void)
{ 
	DSTATUS stat;
	
	spi_init();
	//spi_start();
	LCD_init();
	LCD_wake();

	
	
    while (1) 
    {
		LCD_clr();
		stat = disk_initialize();
		if (!stat) { //0x00 disk_initialize() return indicates successful initialization
			LCD_str("SD init success!");
		}
		else {
			LCD_str("SD init fail!");
		}
		//LCD_cmd(0xC0);
		//LCD_cmd(0x80);
		_delay_ms(500);
    }
}

