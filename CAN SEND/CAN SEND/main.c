/*
 * CAN SEND.c
 *
 * Created: 8/22/2020 1:23:16 PM
 * Author : Waylon
 */ 
#define F_CPU 16000000UL
//#define F_CPU 20000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "CAN.h"
#include "MCP2515_DEFS.h"

int main(void)
{
    /* Initialize SPI as a master device, on frequency < 10Mhz */
    initMCP2515();
    sei();
	
	char data[8] = {0x01, 0x02, 0x03};
    while (1) 
    {
		sendCANmsg(0, 0x7E1, data, 3); 
		_delay_ms(1000);
    }
}

