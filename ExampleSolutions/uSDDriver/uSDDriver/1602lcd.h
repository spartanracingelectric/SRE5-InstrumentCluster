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

//#define F_CPU 16000000UL //Defined in conf.h

#include "conf.h"
#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "i2c.h"

#define LCD_TIMEOUT 6 //In seconds

void LCD_write(unsigned char data, uint8_t mode); /* Writes byte via I2C.  */
void LCD_cmd(unsigned char cmd);
void LCD_char(unsigned char data);
void LCD_hex(unsigned char data);
void LCD_init();
void LCD_str(char *str);
void LCD_str_xy (char row, char pos, char *str);
void LCD_int(int num);
void LCD_clr();
void LCD_wake();
void LCD_update();
void LCD_timestamp();
void LCD_default();
void LCD_menu();
void LCD_back();
void LCD_settings();
void LCD_optionx();
void LCD_optiony();
uint8_t LCD_get_state();

#endif /* 1602LCD_H_ */