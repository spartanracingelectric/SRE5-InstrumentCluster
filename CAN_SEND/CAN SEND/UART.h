/*
 * UART.h
 *
 * Created: 8/22/2020 1:26:18 PM
 *  Author: Waylon
 */ 


#ifndef UART_H_
#define UART_H_

#include <avr/io.h>
#define BAUD 9600
#define BRC ((F_CPU/16/BAUD) - 1)

void UART_INIT(void)
{
UBRR0H = (BRC >> 8);
UBRR0L = BRC;

UCSR0B = (1<<TXEN0);
UCSR0C = (1<<UCSZ01) | (1<<UCSZ00);	
}

void write_uart(char data)
{
	char input = data;
	UDR0 = input;
}
#endif /* UART_H_ */