/* 
 * SRE Instrument Cluster 1602 LCD Driver
 *
 * LCD COMMANDS http://www.dinceraydin.com/lcd/commands.htm
 * HD44780 DATASHEET https://www.makerguides.com/wp-content/uploads/2019/02/HD44780-Datasheet.pdf
 * PCF8574 DATASHEET https://www.makerguides.com/wp-content/uploads/2019/02/PCF8574-Datasheet.pdf
 * PCF8574T to HD44780 PINOUT https://www.playembedded.org/blog/hd44780-backpack-stm32/
 */

#include "1602lcd.h"

#define INIT_MODE 0x80
#define CMD_MODE 0x00
#define DATA_MODE 0x01

int extraTime = 0;

extern uint8_t state = 1;

void LCD__write(unsigned char data, uint8_t mode) {
	
	uint8_t dataTemp;
	
	i2c__start(LCD_ADDRESS);
    
    // Send upper nibble
	dataTemp = (data & 0xF0) | LCD_BL | LCD_E; //Loads upper nibble onto last four bits P4-P7
	if (mode == CMD_MODE)
    {
		dataTemp = (dataTemp & ~LCD_RS); //Set RS to 0 to signify command mode and Toggle E on -> When enable is toggled on, LCD knows to execute instructions given
    }        
	else
    {	
		dataTemp |= LCD_RS; //Set RS to 1 to signify character mode
    }  
	i2c__write(dataTemp); //Send upper nibble while toggling E ON
    _delay_ms(1);
    dataTemp &= ~LCD_E; //Toggle E off
	i2c__write(dataTemp); //Send "toggle E OFF"
    _delay_ms(1);
    
    // Send lower nibble 
	dataTemp = (data << 4) | LCD_BL | LCD_E; //Loads lower nibble onto last four bits P4-P7 and Toggle E on
	if (mode == CMD_MODE)
	{
    	dataTemp = (dataTemp & ~LCD_RS); //Set RS to 0 to signify command mode and Toggle E on -> When enable is toggled on, LCD knows to execute instructions given
	}
	else
	{
    	dataTemp |= LCD_RS; //Set RS to 1 to signify character mode
	}
    i2c__write(dataTemp); //Lower nibble
    _delay_ms(1);
	dataTemp &= ~LCD_E; //Toggle E off
	i2c__write(dataTemp); //Send "toggle E off"
    _delay_ms(1);
    
	i2c__stop();
}

void LCD__cmd(unsigned char cmd)
{
	LCD__write(cmd, CMD_MODE);
}

void LCD__char(unsigned char data)
{
	LCD__write(data, DATA_MODE);
}

void LCD__hex(unsigned char data) //Displays byte in the form of two hex nibbles/digits
{
	unsigned char upperNibble, lowerNibble;
	upperNibble = (data & 0xF0) >> 4;
	if (upperNibble <= 9) {
		upperNibble += 48;
	}
	else {
		upperNibble += 55;
	}
	lowerNibble = data & 0x0F;
	if (lowerNibble <= 9) {
		lowerNibble += 48;
	}
	else {
		lowerNibble += 55;
	}
	LCD__char(upperNibble);
	LCD__char(lowerNibble);
}

/* LCD Initialize function, INITIALIZE LCD PORTS HERE! */
void LCD__init()
{	
	
	TCCR0A = (1<<WGM01); //Set CTC bit
	OCR0A = 156; //Timer ticks per .01s, 16MHz Clk/1024
	TIMSK0 = (1 << OCIE0A);
	TCCR0B = (1<<CS02) | (1<<CS00); //Set Clk/1024
	
	//LCD_DDR = 0xFF;			/* Make LCD port direction as o/p */ //4 bit mode, sets PORTD on MCU as output. Perhaps unnecessary in I2C since it's through SCL/SDA
	i2c__init();
	
	_delay_ms(20);			/* LCD Power ON delay always >15ms */
	
    // We send this three times so that we can configure 4-bit mode after soft reset
	LCD__cmd(0x02);		/* 1st send for 4 bit initialization of LCD  */
    _delay_ms(5);
    LCD__cmd(0x02);		/* 2nd send for 4 bit initialization of LCD  */
    _delay_ms(1);
    LCD__cmd(0x02);		/* 3rd send for 4 bit initialization of LCD  */
    _delay_ms(1); 
	LCD__cmd(0x28);              /* 2 line, 5 x 8 dot char font in 4-bit mode */
	LCD__cmd(0x0C);				/* Turns cursor off */
	LCD__cmd(0x01);              /* Clear display screen*/
    LCD__cmd(0x02);              /* Return home*/
}

void LCD__str(char *str)		/* Send string to LCD function */
{
	int i;
	for(i=0;str[i]!=0;i++)		/* Send each char of string till the NULL */
	{
		LCD__char (str[i]);
	}
}

void LCD_str_xy (char row, char pos, char *str)	/* Send string to LCD with xy position */
{
	if (row == 0 && pos<16)
	LCD__cmd((pos & 0x0F)|0x80);	/* Command of first row and required position<16 */
	else if (row == 1 && pos<16)
	LCD__cmd((pos & 0x0F)|0xC0);	/* Command of first row and required position<16 */
	LCD__str(str);		/* Call LCD string function */
}

void LCD_int(int num) {
	char buff[3];
	itoa(num, buff, 10);
	LCD__str(buff);
}

void LCD_clr()
{
	LCD__cmd (0x01);		/* Clear display */
	_delay_ms(2);
	LCD__cmd (0x80);		/* Cursor at home position */
}

void LCD_clr_ln(int lineNo){ //1st line = 0, 2nd line = 1
	if (lineNo == 0)
		LCD__cmd(0x80);
	else
		LCD__cmd(0xC0);
	LCD__str("                ");
	LCD__cmd(0x80);
}

void LCD__wake() {
	LCD__clr();
	LCD__cmd(0x80); //1st line
	
	for(int k = 177; k < 182; k++) { //177-182 to print A I U E O
		LCD__clr();
		LCD__cmd(0x80); //1st line
		for (int l = 0; l < 16; l++) {
			LCD__char(k);
			//_delay_ms(5);
		}
		LCD__cmd(0xC0); //2nd line
		for (int m = 0; m < 16; m++) {
			LCD__char(k);
			//_delay_ms(5);
		}
		_delay_ms(250); //Wait 350ms per character change
	}
	LCD__cmd(0x80); //1st line
	for (int i = 0; i < 16; i++) { //print black bars on first line
		LCD__char(0xFF);
		_delay_ms(10);
	}
	LCD__cmd(0xC0); //2nd line
	for (int j = 0; j < 16; j++) { //print black bars on second line
		LCD__char(0xFF);
		_delay_ms(10);
	}
	_delay_ms(500); //Wait a 500ms before clearing
	LCD__clr();
	LCD__cmd(0x83);
	LCD__str("LCD Ready!");
	_delay_ms(400);
	LCD__clr();

}

void LCD_update() {
	//Called by CAN interrupt

	LCD__cmd(0x84);
	LCD__str("    ");
	LCD__cmd(0x8D);
	LCD__str("  ");
}

void LCD_timestamp() {
	LCD__cmd(0xC2);
	LCD__str("TMSTMP REC!");
	_delay_ms(400);
	LCD_clr_ln(1); //clear 2nd line	
}

void LCD__default() {
	state = 1;
	LCD__clr();
	LCD__str("SOC:");
	LCD__cmd(0x89);
	LCD__str("Bat:");
	LCD__cmd(0x8F);
	LCD__char(0b11011111); //Degree
}

void LCD__menu() {
	state = 2;
	extraTime = 0;
	LCD__clr();
	LCD__str("Stgs.      Op. X");
	LCD__cmd(0xC0);
	LCD__str("Back       Op. Y");
}

void LCD__back() {
	if (state >= 3)
		LCD__menu();
	else
		LCD__default();
}

void LCD__settings() {
	state = 3;
	extraTime = 0;
	LCD__clr();
	LCD__str("Settings");
	LCD__cmd(0xC0);
	LCD__str("Back");
}

void LCD__optionx() {
	state = 4;
	extraTime = 0;
	LCD__clr();
	LCD__str("Option X");
	LCD__cmd(0xC0);
	LCD__str("Back");
}

void LCD__optiony() {
	state = 5;
	extraTime = 0;
	LCD__clr();
	LCD__str("Option Y");
	LCD__cmd(0xC0);
	LCD__str("Back");
}

ISR(TIMER0_COMPA_vect) { //Interrupt for button
	
	if (state >= 2) {
		extraTime++;
		if (extraTime > TIMEOUT*100) //600*.01s = 6s
			LCD__default(); //Return to default
	}
}