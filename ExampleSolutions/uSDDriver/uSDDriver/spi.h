#ifndef SPI_H
#define SPI_H

#define F_CPU 16000000UL //Change MCU speed here, delay.h dependent

#include <avr/io.h>
#include <util/delay.h>
#include "1602lcd.h"

#define LED  2

#define CS	 2
#define MOSI 3
#define MISO 4
#define SCK  5

/* ARDUINO PRO MINI PINOUT:
 * CS:   PB2 (pin 10)
 * SCK:  PB5 (pin 13)
 * MOSI: PB3 (pin 11)
 * MISO: PB4 (pin 12)
 */

void spi_init(void);
void spi_start(void);
void spi_stop(void);
void spi_write(unsigned char data);
unsigned char spi_receive(void);
unsigned char spi_tranceiver (unsigned char data);







#endif /* SPI_H */