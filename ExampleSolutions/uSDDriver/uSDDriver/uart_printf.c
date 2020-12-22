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
		uart___polled_put(message[i]); 
	}
}