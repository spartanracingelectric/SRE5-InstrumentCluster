#define F_CPU 16000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "CAN.h"


int main(void)
{
	stdout = &mystdout;
	uart__init(9600);
	if (mcp2515__init() != true)
	{
		printf("Init failed\n");
	}else
	{
		printf("Init Sucessful\n");
		
	}
	
    while (1) 
    {
		_delay_ms(500);
    }
}

