#include "spi.h"

/* ARDUINO PRO MINI PINOUT:
 * CS:   PB2 (pin 10)
 * SCK:  PB5 (pin 13)
 * MOSI: PB3 (pin 11)
 * MISO: PB4 (pin 12)
 */

void spi_init(void) {
	//PORTD=0x00; //Set port D to LOW
	//DDRD=0xFF; //Set port D to OUTPUT
	
	//MOSI, SCK, CS all high, output in master mode
	//DDRB = (1<<MOSI) | (1<<CS) | (1<<SCK);
	//PORTB = (1<<MOSI) | (1<<CS) | (1<<SCK);
	DDRB = (1<<MOSI) | (1<<CS) | (1<<SCK);
	//PORTB = (1<<MOSI) | (1<<SCK);
	//Definitions defined in main.c
	
	//Set SPI Control Register bits, further explained below
	//SPCR = (1<<MSTR) | (1<<SPR0);
	SPCR = (1<<SPE) | (1<<MSTR) | (1<<SPR0); //ENABLES SPI
	/*
		SPE: SPI Enable bit, 1 = SPI enabled
		MSTR: Select Master bit, 1 = master mode
		SPR0: SPI Clock Rate 0 Select bit, SPR1 0 & SPR0 1 = SCK freq of f_osc/16
			- Sets SCK freq to f_osc/16 setting, which matches MCU clock of 16MHz
		CPOL & CPHA default to 0 so SPI mode is 0, which SDC uses
	*/
	
}

void spi_start(void) {
	//Enable SPI, Set SPI Enable bit to 1;
	SPCR |= (1<<SPE);
}

void spi_stop(void) {
	//Disable SPI, Set SPI Enable bit to 0;
	SPCR &= ~(1<<SPE);
}

void spi_write(unsigned char data) {
	//Load SPI data register with data
	SPDR = data;
	//Wait for data transmission to finish
	while (!(SPSR & (1<<SPIF)));
}

unsigned char spi_receive(void) {
	//Wait for data transmission to finish
	while (!(SPSR & (1<<SPIF)));
	
	return SPDR;
}

unsigned char spi_tranceiver (unsigned char data)
{
	SPDR = data;                                  //Load data into buffer
	while(!(SPSR & (1<<SPIF) ));                  //Wait until transmission complete
	return(SPDR);                                 //Return received data
}

