#ifndef SPI_H
#define SPI_H

//#define F_CPU 16000000UL //Defined in conf.h

#include "conf.h"
#include <avr/io.h>
#include <util/delay.h>

void spi_init(void); //Initialize dedicated SPI I/O
void spi_start(void); //Start SPI
void spi_stop(void); //Stop SPI
void spi_write(unsigned char data); //Write (arg) data via SPDR register
unsigned char spi_receive(void); //Receive (return) data via SPDR register
unsigned char spi_tranceiver (unsigned char data); //Write (arg) and receive (return) data via SPDR register


#endif /* SPI_H */