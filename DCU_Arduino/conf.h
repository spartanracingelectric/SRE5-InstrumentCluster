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
/ I2C ADDRESSES
/---------------------------------------------------------------------------*/

/* 1602 LCD I2C ADDRESS */
#define LCD_ADDRESS 0x38		

/* RPM LED BAR I2C ADDRESS */
#define LED_BAR_1 0x3A
#define LED_BAR_2 0x39

/* INDICATOR LEDS I2C ADDRESS */
#define LED_INDICATORS 0x3B

/*---------------------------------------------------------------------------/
/ I/O PORTS
/---------------------------------------------------------------------------*/
/* SPI PINS */



/*---------------------------------------------------------------------------/
/ BUTTON PINS AND CONSTANTS
/---------------------------------------------------------------------------*/

#define BUT1 14 //PINC0
#define BUT2 15 //PINC1
#define BUT3 16 //PINC2
#define BUT4 17 //PINC3

#define PCINT_BUT1 (1<<PCINT8)
#define PCINT_BUT2 (1<<PCINT9)
#define PCINT_BUT3 (1<<PCINT10)
#define PCINT_BUT4 (1<<PCINT11)

#endif /* CONF_H_ */
