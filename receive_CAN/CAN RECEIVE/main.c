#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "CAN.h"
#include "uart.h"
#include "uart_printf.h"

static int uart_putchar(char c, FILE *stream);
static FILE mystdout = FDEV_SETUP_STREAM(uart_putchar, NULL, _FDEV_SETUP_WRITE);
static int uart_putchar(char c, FILE *stream)
{
	if (c == '\n')
	uart_putchar('\r', stream);
	while(!(UCSR0A & (1<<UDRE0)));
	UDR0 = c;
	return 0;
}


void send_test()
{
	uint8_t data[3] = {0x30, 0x40, 0x50};
	while (1)
	{
		_delay_ms(1000);
		send0(data, 0x0, 3);
	}
	
	
}


int main(void)
{
	stdout = &mystdout;
	uart__init(9600);
	initMCP2515(); //init mcp2515 and reception process
	uart__printf("hello");

	send_test();
	/*
	can_message_t message;
	can_message_t *pointer;
	pointer = &message;
    while (1) 
    {
		if(mcp2515__get(pointer) == true)
		{
			uint16_t id = pointer->id;
			uint8_t	length = pointer->data_length;
			printf("ID: %X \n", id);
			/*
			uart__print_num(id);
			uart__printf("\n");
			*/
			
			/*
			if (id == 0x0D)
			{
				uart__printf("Speed is \n");
				for (int i = 0; i< length; i++)
				{
					uart__print_num(pointer -> data[i]);
					uart__printf("\n");
				}
				_delay_ms(2000);
			}
			if (id ==  0x0C)
			{
				uart__printf("RPM is \n");
				for (int i = 0; i< length; i++)
				{
					uart__print_num(pointer -> data[i]);
					uart__printf("\n");
				}
				_delay_ms(2000);
			}
			*/
			/*
			_delay_ms(2000);
		}
		else
		{
			printf("No Message\n");
			_delay_ms(500);
			//uart__printf("no message\n");
		}
	}
	return 0;
	*/
}

