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
#include "sd.h"

int main(void)
{ 
	FATFS pfs;
	UINT bw;
	
	uart__init(9600);
	
	spi_init();
	
	LCD_init();
	
	buttons_init();
	
	uart__print_welcome();
	uart__print_disk_info();
	
	sd__mount(&pfs);
	sd__open(FILE_NAME);

	LCD_wake();
	LCD_default();
	
	sei(); //Enable interrupts
	
	int count = 0;
	char test[6] = "Test ";
	char temp_str[16];
	char count_char[8];
	
    while (1) 
    {
		memset(temp_str, '\0', 16);
		strcpy(temp_str, test);
		itoa(count, count_char, 10);
		strcat(temp_str, count_char);
		sd__new_entry(temp_str, BUFFER_SIZE, &bw);
		count++;
		_delay_ms(250);
    }
}

