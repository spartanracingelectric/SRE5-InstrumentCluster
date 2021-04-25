#ifndef CONF_H_
#define CONF_H_

/****************************************************************************/
/*  INSTRUMENT CLUSTER - SETUP CONFIGURATION FILE (for ports, etc)			*/
/*																			*/
/*  DUE TO F_CPU, conf.h MUST BE INCLUDED BEFORE ANY <avr/delay.h>			*/
/****************************************************************************/

/* ATMega328P CLOCK SPEED */
#define F_CPU 16000000UL

/*---------------------------------------------------------------------------/
/ ADDRESSES
/---------------------------------------------------------------------------*/

/* 1602 LCD I2C ADDRESS */
#define LCD_ADDRESS 0x38		

/* RPM LED BAR I2C ADDRESS */
#define LED_BAR_1 = 0x3A;
#define LED_BAR_2 = 0x39;

/* INDICATOR LEDS I2C ADDRESS */
#define LED_INDICATORS = 0x3B;

/*---------------------------------------------------------------------------/
/ I/O PORTS
/---------------------------------------------------------------------------*/
/* SPI PINS */

#endif /* CONF_H_ */