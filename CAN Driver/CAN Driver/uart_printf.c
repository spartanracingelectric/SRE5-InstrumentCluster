#include "uart_printf.h"



void uart__printf(char *message)
{
	const size_t message_length = strlen(message); 
	for(size_t i = 0; i < message_length; i++)
	{
		uart__polled_put(message[i]); 
	}
}

void uart__print_new_line() {
	uart__polled_put('\r');
	uart__polled_put('\n');
}


void uart__print_welcome() {
	char r1[39] = "___       __     _         __ ___ _  _";
	char r2[40] = " |  |\\ | (_     /  |  | | (_   | |_ |_)";
	char r3[40] = "_|_ | \\| __) o  \\_ |_ |_| __)  | |_ | \\";

	for(int i = 0; i < 39; i++)
		uart__polled_put('#');
	uart__print_new_line();
	uart__printf(r1);
	uart__print_new_line();
	uart__printf(r2);
	uart__print_new_line();
	uart__printf(r3);
	uart__print_new_line();
	uart__print_new_line();
	for(int i = 0; i < 39; i++)
		uart__polled_put('#');
	uart__print_new_line();
}


void uart__print_hex(unsigned char data) {
	unsigned char upperNibble, lowerNibble;
	upperNibble = (data & 0xF0) >> 4;
	if (upperNibble <= 9) {
		upperNibble += 48;
	}
	else {
		upperNibble += 55;
	}
	lowerNibble = data & 0x0F;
	if (lowerNibble <= 9) {
		lowerNibble += 48;
	}
	else {
		lowerNibble += 55;
	}
	uart__polled_put('0');
	uart__polled_put('x');
	uart__polled_put(upperNibble);
	uart__polled_put(lowerNibble);
}



void uart__print_num(uint8_t number)
{
	uint8_t c; 
	c = (number & 0xf0) >> 4; 
	number = number & 0x0f; 
	if(c < 10)
	{
		c = c + '0'; 
	}
	else
	{
		c = c + '7'; 
	}
	if(number < 10)
	{
		number = number + '0';
	}
	else
	{
		number = number + '7';
	}
	uart__polled_put(c);
	uart__polled_put(number);

}

void uart__print_binary(uint8_t number)
{
	for(int i = 7; i >= 0; i--)
	{
 		if(number & (1 << i))
		{
			uart__polled_put('1');
		}
		else
		{
			uart__polled_put('0');
		}
	}
}

void uart__print_can_id(uint16_t id)
{
	for(int i = 10; i >= 0; i--)
	{
		if(id & (1 << i))
		{
			uart__polled_put('1');
		}
		else
		{
			uart__polled_put('0');
		}
	}
}
