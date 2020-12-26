/*
 * uart_printf.c
 *
 * 
 *  Author: sean
 */ 
#include "uart_printf.h"
#include "uart.h"
#include <string.h>

void uart__printf(char *message)
{
	const size_t message_length = strlen(message); 
	for(size_t i = 0; i < message_length; i++)
	{
		uart__polled_put(message[i]); 
	}
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