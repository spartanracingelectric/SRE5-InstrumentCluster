#ifndef CONF_H_
#define CONF_H_

/****************************************************************************/
/*  INSTRUMENT CLUSTER - SETUP CONFIGURATION FILE (for ports, etc)			*/
/*																			*/
/*  DUE TO F_CPU, conf.h MUST BE INCLUDED BEFORE ANY <avr/delay.h>			*/
/****************************************************************************/

/* ATMega328P CLOCK SPEED */
#define F_CPU 16000000UL

/* SD CARD */
#define SD_USE_READ 1					//0 disables or 1 enables READ
#define FILE_NAME "TEST.TXT"			//File name
#define BUFFER_SIZE 32					//For SD, per line
//#define FILE_SIZE 4000000000 //Max size for FAT32 is 4GB

/*---------------------------------------------------------------------------/
/ ADDRESSES
/---------------------------------------------------------------------------*/

/* 1602 LCD I2C ADDRESS */
//#define I2C_ADDRESS 0x27				//PCF8574T LCD Backpack
#define I2C_ADDRESS 0x20				//PCF8574T 9 Pin Board and PCF8574P
//#define I2C_ADDRESS 0x38				//PCF8574AN

/* RPM LED BAR I2C ADDRESS */


/* RGB LED I2C ADDRESS */


/*---------------------------------------------------------------------------/
/ I/O PORTS
/---------------------------------------------------------------------------*/
/* SPI (USE ATMEGA DEDICATED I/O) */
#define CS	 2
#define MOSI 3
#define MISO 4
#define SCK  5

/* 1602 LCD */
#define LCD_TIMEOUT 6					//In seconds
#define LCD_RS (1<<0)					//PCF P0
#define LCD_E (1<<2)					//P2
#define LCD_BL (1<<3)					//P3 goes to backlight A/K on LCD
#define LCD_D4 (1<<4)					//P4
#define LCD_D5 (1<<5)					//P5
#define LCD_D6 (1<<6)					//P6
#define LCD_D7 (1<<7)					//P7

/* BUTTONS */
#define HOLD_TIME 1000					//Button hold time for bottom buttons on default screen. In ms
#define BUT1 PINC0						//Top left
#define BUT2 PINC1						//Top right
#define BUT3 PINC2						//Bottom left
#define BUT4 PINC3						//Bottom right
#define PCINT_BUT1 (1<<PCINT8)
#define PCINT_BUT2 (1<<PCINT9)
#define PCINT_BUT3 (1<<PCINT10)
#define PCINT_BUT4 (1<<PCINT11)



#endif /* CONF */
