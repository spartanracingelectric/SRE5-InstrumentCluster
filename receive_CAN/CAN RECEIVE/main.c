#define F_CPU 20000000ul
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "CAN.h"
#include "MCP2515_DEFS.h"




int main(void)
{
	init_MCP2515(); //init mcp2515 and reception process
	
    while (1) 
    {
		if (RXB0_Flag == 1)
		{
			RXB0_Flag =0;
			
		}
    }
	return 0;
}

