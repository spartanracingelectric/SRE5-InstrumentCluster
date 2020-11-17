/*
 * uart.c
 *
 * Created: 11/8/2020 8:16:44 PM
 *  Author: sean
 */ 

#include "uart.h"
#include <stdbool.h>
#define F_CPU 16000000UL

void uart__init(uint32_t baud_rate)
{
	// UBRn fosc/(16 Baud) - 1
	uint32_t uart_baud_rate; 
	if(baud_rate != 9600)
	{
		uart_baud_rate = F_CPU / (16 * baud_rate) - 1;
	}
	else
	{
		uart_baud_rate = (F_CPU / 8 / baud_rate - 1) / 2;
	}
	UBRR0H = (uint8_t)(uart_baud_rate >> 8);
	UBRR0L = (uint8_t)uart_baud_rate;

	// Enable reciever and transmitter
	UCSR0B = (1 << RXEN0) | (1 << TXEN0);
	// Set Frame: 8 data bit, 1 stop bit
	UCSR0C = (1 << USBS0) | (3 << UCSZ00);
}

bool uart__is_ready(void)
{
	// The transmit buffer is empty and is ready to transmit
	return UCSR0A & (1 << UDRE0); 
}


bool uart___polled_put(uint8_t data)
{
	// Interrupt is disabled, we need to poll data 
	bool status = false;
	while(!uart__is_ready())
	;
	UDR0 = data;
	return status; 
}

uint8_t uart__receive(void)
{
	while(!(UCSR0A & (1 << RXC0)))
	;
	return UDR0;
}


