#include "conf.h" //Has F_CPU defined
#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include "uart.h"
#include "uart_printf.h"
#include "spi.h"
#include "1602lcd.h"
#include "buttons.h"
#include "i2c.h"
#include "diskio.h"
#include "pff.h"

int main(void)
{ 
	char c[100];
	
	FATFS pfs;
	UINT br;
	FRESULT res;
	
	uart__init(9600);
	spi_init();
	//spi_start();
	
	LCD_init();
	buttons_init();
	
	uart__print_welcome();
	
	uart__print_disk_info();
	
	res = pf_mount(&pfs);
	uart__print_disk_error(res, MOUNT);
	
	res = pf_open(FILE_NAME); //File name found in conf.h
	uart__print_disk_error(res, OPEN);

	LCD_wake();

	LCD_default();
	
	sei(); //Enable interrupts
	
	res = pf_read(c, sizeof(c), &br);
	uart__print_disk_error(res, READ);
	
    while (1) 
    {
		uart__printf(c);
		uart__print_new_line();
		_delay_ms(2000);
    }
}

