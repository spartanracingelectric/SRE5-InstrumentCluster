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

void LCD_write(unsigned char data, uint8_t mode); /* Writes byte via I2C. Mode is for LCD data transmission */
void LCD_cmd(unsigned char cmd); /* Sends byte to LCD in LCD_CMD_MODE mode */
void LCD_char(unsigned char data); /* Sends byte to LCD in LCD_DATA_MODE mode. Displays char */
void LCD_hex(unsigned char data); /* Displays two hex characters representing a byte */
void LCD_init(); /* Initializes LCD. Essential. */
void LCD_str(char *str); /* Uses LCD_char. Displays string. */
void LCD_str_xy (char row, char pos, char *str); /* Displays string at a certain row/col position */ 
void LCD_int(int num); /* Displays an integer given an integer as an argument */
void LCD_clr(); /* Clears LCD screen, resets cursor to start */
void LCD_wake(); /* Testing functionality for LCD */
void LCD_update(); /* Called by CAN interrupt, needs implementation */
void LCD_timestamp(); /* Activated by button, records timestamp. Needs implementation */
void LCD_default(); /* Default Instrument Cluster screen. Shows SOC & Battery Temp */
void LCD_menu(); /* Main menu to enter sub menus */
void LCD_back(); /* Back out one menu level */
void LCD_settings(); /* Submenu, needs implementation */
void LCD_optionx(); /* Submenu, needs implementation */
void LCD_optiony(); /* Submenu, needs implementation */

uint8_t LCD_get_state(); /* Get function for LCD menu state. buttons.h needs it */

#endif /* 1602LCD_H_ */