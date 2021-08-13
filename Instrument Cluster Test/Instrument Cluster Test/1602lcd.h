#ifndef LCD1602_H_
#define LCD1602_H_

#include <avr/io.h>
#include <util/delay.h>
#include "i2c.h"

#define INIT_MODE 0x80
#define CMD_MODE 0x00
#define DATA_MODE 0x01

uint8_t I2C_ADDRESS,
LCD_RS_PORT,
LCD_E_PORT,
LCD_BL_PORT,
LCD_D4_PORT,
LCD_D5_PORT,
LCD_D6_PORT,
LCD_D7_PORT;

void LCD_write(unsigned char data, uint8_t mode);
void LCD_cmd(unsigned char cmd);
void LCD_char(unsigned char data);
void LCD_init(uint8_t i2cAddress, uint8_t RS, uint8_t E, uint8_t BL, uint8_t D4, uint8_t D5, uint8_t D6, uint8_t D7);
void LCD_str(char *str);
void LCD_str_xy (char row, char pos, char *str);
void LCD_clr();
void LCD_wake();

void LCD_write(unsigned char data, uint8_t mode) {
	
	uint8_t dataTemp;
	
	i2c__start(I2C_ADDRESS);
	
	// Send upper nibble
	dataTemp = (data & 0xF0) | LCD_BL_PORT | LCD_E_PORT; //Loads upper nibble onto last four bits P4-P7
	if (mode == CMD_MODE)
	{
		dataTemp = (dataTemp & ~LCD_RS_PORT); //Set RS to 0 to signify command mode and Toggle E on -> When enable is toggled on, LCD knows to execute instructions given
	}
	else
	{
		dataTemp |= LCD_RS_PORT; //Set RS to 1 to signify character mode
	}
	i2c__write(dataTemp); //Send upper nibble while toggling E ON
	_delay_ms(1);
	dataTemp &= ~LCD_E_PORT; //Toggle E off
	i2c__write(dataTemp); //Send "toggle E OFF"
	_delay_ms(1);
	
	// Send lower nibble
	dataTemp = (data << 4) | LCD_BL_PORT | LCD_E_PORT; //Loads lower nibble onto last four bits P4-P7 and Toggle E on
	if (mode == CMD_MODE)
	{
		dataTemp = (dataTemp & ~LCD_RS_PORT); //Set RS to 0 to signify command mode and Toggle E on -> When enable is toggled on, LCD knows to execute instructions given
	}
	else
	{
		dataTemp |= LCD_RS_PORT; //Set RS to 1 to signify character mode
	}
	i2c__write(dataTemp); //Lower nibble
	_delay_ms(1);
	dataTemp &= ~LCD_E_PORT; //Toggle E off
	i2c__write(dataTemp); //Send "toggle E off"
	_delay_ms(1);
	
	i2c__stop();
}

void LCD_cmd(unsigned char cmd)
{
	LCD_write(cmd, CMD_MODE);
}

void LCD_char(unsigned char data)
{
	LCD_write(data, DATA_MODE);
}

/* LCD Initialize function, INITIALIZE LCD PORTS HERE! */
void LCD_init(uint8_t i2cAddress, uint8_t RS, uint8_t E, uint8_t BL, uint8_t D4, uint8_t D5, uint8_t D6, uint8_t D7)
{
	LCD_RS_PORT = RS;
	LCD_E_PORT =  E;
	LCD_BL_PORT = BL;
	LCD_D4_PORT = D4;
	LCD_D5_PORT = D5;
	LCD_D6_PORT = D6;
	LCD_D7_PORT = D7;
	I2C_ADDRESS = i2cAddress;
	
	//LCD_DDR = 0xFF;			/* Make LCD port direction as o/p */ //4 bit mode, sets PORTD on MCU as output. Perhaps unnecessary in I2C since it's through SCL/SDA
	i2c__init();
	
	_delay_ms(20);			/* LCD Power ON delay always >15ms */
	
	// We send this three times so that we can configure 4-bit mode after soft reset
	LCD_cmd(0x02);		/* 1st send for 4 bit initialization of LCD  */
	_delay_ms(5);
	LCD_cmd(0x02);		/* 2nd send for 4 bit initialization of LCD  */
	_delay_ms(1);
	LCD_cmd(0x02);		/* 3rd send for 4 bit initialization of LCD  */
	_delay_ms(1);
	LCD_cmd(0x28);              /* 2 line, 5 x 8 dot char font in 4-bit mode */
	LCD_cmd(0x0C);				/* Turns cursor off */
	LCD_cmd(0x01);              /* Clear display screen*/
	LCD_cmd(0x02);              /* Return home*/
}

void LCD_str(char *str)		/* Send string to LCD function */
{
	int i;
	for(i=0;str[i]!=0;i++)		/* Send each char of string till the NULL */
	{
		LCD_char (str[i]);
	}
}

void LCD_str_xy (char row, char pos, char *str)	/* Send string to LCD with xy position */
{
	if (row == 0 && pos<16)
	LCD_cmd((pos & 0x0F)|0x80);	/* Command of first row and required position<16 */
	else if (row == 1 && pos<16)
	LCD_cmd((pos & 0x0F)|0xC0);	/* Command of first row and required position<16 */
	LCD_str(str);		/* Call LCD string function */
}

void LCD_clr()
{
	LCD_cmd (0x01);		/* Clear display */
	_delay_ms(2);
	LCD_cmd (0x80);		/* Cursor at home position */
}

void LCD_wake() {
	LCD_clr();
	LCD_cmd(0x80); //1st line
	
	for(int k = 177; k < 182; k++) { //177-182 to print A I U E O
		LCD_clr();
		LCD_cmd(0x80); //1st line
		for (int l = 0; l < 16; l++) {
			LCD_char(k);
			//_delay_ms(5);
		}
		LCD_cmd(0xC0); //2nd line
		for (int m = 0; m < 16; m++) {
			LCD_char(k);
			//_delay_ms(5);
		}
		_delay_ms(350); //Wait 350ms per character change
	}
	LCD_cmd(0x80); //1st line
	for (int i = 0; i < 16; i++) { //print black bars on first line
		LCD_char(0xFF);
		_delay_ms(30);
	}
	LCD_cmd(0xC0); //2nd line
	for (int j = 0; j < 16; j++) { //print black bars on second line
		LCD_char(0xFF);
		_delay_ms(30);
	}
	_delay_ms(1000); //Wait a sec before clearing
	LCD_clr();
}

#endif /* 1602LCD_H_ */