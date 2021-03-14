/*
 *  spi.c
 *
 * 
 *  Author: sean
 */ 
#include "spi.h"

#include <avr/io.h>

void spi_init(void)
{
	// Reset SPI pins to prevent run time error
	PORTB &= ~(1 << PINB3); 
	PORTB &= ~(1 << PINB4); 
	PORTB &= ~(1 << PINB5); 
	
	// Default is 8-bit mode and MSB transmits first
	DDRB |= (1 << PINB3) | (1 << PINB5); // Set MOSI and CLK as output 
	DDRB &= ~(1 << PINB4); // set MISO as input 
	
	SPCR = (1 << SPE) | (1 << MSTR); // Enable SPI and set it to master mode
	
	// Set SPI speed to 1MHz (fosc / 16). When CPU speed set to 16MHz, 1 MHz SPI is running stably 
	SPCR |= ((0 << SPR1) | (1 << SPR0)); 
	SPSR = 0;
}

uint8_t spi_exchange_byte(uint8_t byte_to_transmit)
{
	SPDR = byte_to_transmit; 
	while (!(SPSR & (1 << SPIF))); 
	return SPDR;
}