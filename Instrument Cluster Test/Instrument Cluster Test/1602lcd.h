#ifndef LCD1602_H_
#define LCD1602_H_

/* 
 * SRE Instrument Cluster 1602 LCD Driver
 *
 * DEFINE CORRECT LCD I2C ADDRESS!!!
 *
 * LCD COMMANDS http://www.dinceraydin.com/lcd/commands.htm
 * HD44780 DATASHEET https://www.makerguides.com/wp-content/uploads/2019/02/HD44780-Datasheet.pdf
 * PCF8574 DATASHEET https://www.makerguides.com/wp-content/uploads/2019/02/PCF8574-Datasheet.pdf
 * PCF8574T to HD44780 PINOUT https://www.playembedded.org/blog/hd44780-backpack-stm32/
 */

#define F_CPU 16000000UL //Change MCU speed here, delay.h and LCD timings dependent

#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "i2c.h"

#define TIMEOUT 6 //6s

#define LCD_RS (1<<0) //PCF P0
#define LCD_E  (1<<2) //P2
#define LCD_BL (1<<3) //P3 goes to backlight A/K on LCD
#define LCD_D4 (1<<4) //P4
#define LCD_D5 (1<<5) //P5
#define LCD_D6 (1<<6) //P6
#define LCD_D7 (1<<7) //P7

//LCD I2C address already defined in i2c.h
//#define I2C_ADDRESS 0x27 //PCF8574T LCD Backpack
//#define I2C_ADDRESS 0x20 //PCF8574T 9 Pin Board and PCF8574P
//#define I2C_ADDRESS 0x38 //PCF8574AN

extern uint8_t state;

void LCD__write(unsigned char data, uint8_t mode);
void LCD__cmd(unsigned char cmd);
void LCD__char(unsigned char data);
void LCD__hex(unsigned char data);
void LCD__init();
void LCD__str(char *str);
void LCD__str_xy (char row, char pos, char *str);
void LCD__clr();
void LCD__wake();
void LCD__update();
void LCD__timestamp();
void LCD__default();
void LCD__menu();
void LCD__back();
void LCD__settings();
void LCD__optionx();
void LCD__optiony();

#endif /* 1602LCD_H_ */