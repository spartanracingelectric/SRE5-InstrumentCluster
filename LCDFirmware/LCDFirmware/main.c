#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>

#define LCD_DDR DDRD //direction
#define LCD_PORT PORTD
#define RS 2
#define E 3
#define D4 4
#define D5 5
#define D6 6
#define D7 7

//Commented ports are Arduino Nano ports
const uint8_t LCD_RS_PORT = (1<<RS), //D2
			  LCD_E_PORT  = (1<<E), //D3
			  LCD_D4_PORT = (1<<D4), //D4
			  LCD_D5_PORT = (1<<D5), //D5
			  LCD_D6_PORT = (1<<D6), //D6
			  LCD_D7_PORT = (1<<D7); //D7

void LCD_cmd(unsigned char cmd) {
	LCD_PORT = (LCD_PORT & 0x0F) | (cmd & 0xF0); //Upper nibble
	LCD_PORT &= ~LCD_RS_PORT;
	LCD_PORT |= LCD_E_PORT;
	_delay_us(1);
	LCD_PORT &= ~LCD_E_PORT;
	
	_delay_us(200);
	
	LCD_PORT = (LCD_PORT & 0x0F) | (cmd << 4); //Lower nibble
	LCD_PORT |= LCD_E_PORT;
	_delay_us(1);
	LCD_PORT &= ~LCD_E_PORT;
	_delay_ms(2);
}

void LCD_char( unsigned char data )
{
	LCD_PORT = (LCD_PORT & 0x0F) | (data & 0xF0); //Send upper nibble
	LCD_PORT |= LCD_RS_PORT;		// RS = 1
	LCD_PORT|= LCD_E_PORT;
	_delay_us(1);
	LCD_PORT &= ~LCD_E_PORT;

	_delay_us(200);

	LCD_PORT = (LCD_PORT & 0x0F) | (data << 4); //Send lower nibble
	LCD_PORT |= LCD_E_PORT;
	_delay_us(1);
	LCD_PORT &= ~LCD_E_PORT;
	_delay_ms(2);
}

void LCD_init (void)			/* LCD Initialize function */
{
	LCD_DDR = 0xFF;			/* Make LCD port direction as o/p */
	_delay_ms(20);			/* LCD Power ON delay always >15ms */
	
	LCD_cmd(0x02);		/* send for 4 bit initialization of LCD  */
	LCD_cmd(0x28);              /* 2 line, 5*7 matrix in 4-bit mode */
	LCD_cmd(0x0c);              /* Display on cursor off*/
	LCD_cmd(0x06);              /* Increment cursor (shift cursor to right)*/
	LCD_cmd(0x01);              /* Clear display screen*/
	_delay_ms(2);
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


int main(void)
{
	
	LCD_init();			/* Initialization of LCD*/

	LCD_str("Hello");	/* Write string on 1st line of LCD*/
	LCD_cmd(0xC0);		/* Go to 2nd line*/
	LCD_str("Hello World");	/* Write string on 2nd line*/
	while(1);
}

