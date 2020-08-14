#ifndef SPI_H_
#define SPI_H_

#include <avr/io.h>

//Defining pins 
#define SS		2 //Slave Select		PB2
#define MOSI	3 //Master-Out Slave-In PB3
#define MISO	4 //Master-In Slave-Out PB4
#define SCK		5 //Serial CLock		PB5

void spi_init(void);
void tx_spi(uint8_t);
uint8_t spi(uint8_t)
uint8_t rx_spi(void)

void spi_init() {
	DDRB |= (1<<SS) | (1<<MOSI) | (1<<SCK); //Set SS, MOSI, and SCK as outputs in master mode
	PORTB |= (1<<MISO); //Pull up on MISO (SD card DO)
	/*
	SPCR is the SPI Control Register
	SPE is the SPI enable pin, set to HIGH to enable SPI
	MSTR is the Master/Slave Select pin, set to HIGH to set Master, LOW sets Slave
	SPR0 and SPR1 set the clock speeds, check data sheet for more info
	*/
	SPCR |= (1<<SPE) | (1<<MSTR) | (1<<SPR1) | (1<<SPR0);	
}

uint8_t spi(uint8_t data) {
	/*
	SPDR is the SPI Data Register
	This is used for data transfer between the register file and SPI shift register
	Writing to this register initiates data transmission
	*/
	SPDR = data;
	/*
	SPSR is the SPI Status Register
	SPIF is the SPI Interrupt flag, it is set to 1 when serial transfer is complete
	*/
	while(!(SPSR & (1<<SPIF)));
	//This line is used for whenever we read 
	return SPDR;
}

void tx_spi(uint8_t data) {
	spi(data);
}

void rx_spi(void) {
	return spi(0xFF)
}
#endif /* SPI_H_ *