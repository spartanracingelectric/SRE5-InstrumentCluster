#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include "spi.h"
#include "1602lcd.h"
#include "i2c.h"
#include "diskio.h"
#include "pff.h"

//#define ACK 0x7E

/* ARDUINO PRO MINI PINOUT:
 * CS:   PB2 (pin 10)
 * SCK:  PB5 (pin 13)
 * MOSI: PB3 (pin 11)
 * MISO: PB4 (pin 12)
 */


int main(void)
{ 
	char c[16];
	
	FATFS pfs;
	UINT br;
	FRESULT res;
	
	spi_init();
	//spi_start();
	LCD_init();
	//LCD_wake();

	//disk_display_init_info();
	//_delay_ms(500);
	LCD_clr();
	res = pf_mount(&pfs);
	if (!res) //No error
		LCD_str("Mount success!");
	else {
		//LCD_str("Mount fail!");
		LCD_cmd(0xC0);
		LCD_hex(res);
	}
	_delay_ms(250);
	LCD_clr();

	res = pf_open("TEST.TXT");
	if (res == FR_OK) {
		LCD_str("File opened!");
		pf_read(c, 16, &br);
	}
	else {
		LCD_str("File open fail!");
		LCD_cmd(0xC0);
		LCD_hex(res);
	}
	_delay_ms(500);
	
	LCD_cmd(0xC0);
	LCD_str(c);
	
    while (1) 
    {
		
		
		//_delay_ms(250);
	
		//LCD_cmd(0xC0);
		//LCD_cmd(0x80);
    }
}

