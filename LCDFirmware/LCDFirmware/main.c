/* LCD COMMANDS http://www.dinceraydin.com/lcd/commands.htm
 * HD44780 DATASHEET https://www.makerguides.com/wp-content/uploads/2019/02/HD44780-Datasheet.pdf
 * PCF8574 DATASHEET https://www.makerguides.com/wp-content/uploads/2019/02/PCF8574-Datasheet.pdf
 * PCF8574T to HD44780 PINOUT https://www.playembedded.org/blog/hd44780-backpack-stm32/
 */

#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <util/twi.h>

//#define LCD_DDR DDRD //direction
//#define LCD_PORT PORTD
#define RS 0 //PCF P0
#define E 2 //P2
#define BL 3 //P3 goes to backlight A/K on LCD
#define D4 4 //P4
#define D5 5 //P5
#define D6 6 //P6
#define D7 7 //P7
#define I2C_ADDRESS 0x27 //PCF8574T
#define SCL_FREQUENCY 100000
#define PRESCALER 1
#define INIT_MODE 0x80
#define CMD_MODE 0x00
#define DATA_MODE 0x01

const uint8_t LCD_RS_PORT = (1<<RS), //(1<<0)
			  LCD_E_PORT  = (1<<E),	 //(1<<2)
			  LCD_BL_PORT = (1<<BL), //(1<<3)
			  LCD_D4_PORT = (1<<D4), //(1<<4)
			  LCD_D5_PORT = (1<<D5), //(1<<5)
			  LCD_D6_PORT = (1<<D6), //(1<<6)
			  LCD_D7_PORT = (1<<D7); //(1<<7)

void twi_init(void)
{
	PRR &= ~(1<<PRTWI); //The PRTWI bit in Section 9.10 “Minimizing Power Consumption" on page 36 must be written to zero to enable the 2-wire serial interface
	TWCR &= ~(1<<TWIE); // If the TWIE bit is cleared, the application must poll the TWINT flag in order to detect actions on the TWI bus
	TWSR &= ~(1<<TWPS0)|(1<<TWPS1); //Sets prescaler value to 1
	TWBR = 72; //Sets bit rate in TWBR; 72 = 100khz bit rate; 12 =400khz bit rate; calcluate with TWBR_val (((F_CPU / F_SCL) - 16 ) / (2*Prescaler))
}

void twi_start(void)
{
	TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);	//Send START condition
	while (!(TWCR & (1<<TWINT)));	//Wait for TWINT flag set. This indicates that the START condition has been transmitted
	if ((TWSR & 0xF8) != TW_START) { //Check value of TWI status register. Mask prescaler bits. If status different from START go to ERROR
	//error();
	}
	TWDR = (I2C_ADDRESS << 1) | TW_WRITE;; //Sets address/data register to slave address + write
	TWCR = (1<<TWINT) | (1<<TWEN); //Clear TWINT bit in TWCR to start transmission of address
	while (!(TWCR & (1<<TWINT))); //Wait for TWINT flag set. This indicates that the SLA+W has been transmitted, and ACK/NACK has been received.
	if ((TWSR & 0xF8) != TW_MT_SLA_ACK) { //Check value of TWI status register. Mask prescaler bits. If status different from TW_MT_SLA_ACK go to ERROR
	//error();
	}
}

void twi_stop(void)
{
	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO);  //Transmit STOP condition
}

void twi_write(unsigned char data)
{
	TWDR = data;  //Load DATA into TWDR register
	TWCR = (1<<TWINT) | (1<<TWEN);  //Clear TWINT bit in TWCR to start transmission of data
	while(!(TWCR & (1<<TWINT)));  //Wait for TWINT flag set. This indicates that the DATA has been transmitted, and ACK/NACK has been received
	if ((TWSR & 0xF8)!= TW_MT_DATA_ACK) {  //Check value of TWI status register. Mask prescaler bits. If status different from TW_MT_DATA_ACK go to ERROR
		//error();
	}
}

void LCD_write(unsigned char data, uint8_t mode) {
	
	uint8_t dataTemp;
	
	twi_start();
    
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
	twi_write(dataTemp); //Send upper nibble while toggling E ON
    _delay_ms(1);
    dataTemp &= ~LCD_E_PORT; //Toggle E off
	twi_write(dataTemp); //Send "toggle E OFF"
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
    twi_write(dataTemp); //Lower nibble
    _delay_ms(1);
	dataTemp &= ~LCD_E_PORT; //Toggle E off
	twi_write(dataTemp); //Send "toggle E off"
    _delay_ms(1);
    
	twi_stop();
}

void LCD_cmd(unsigned char cmd)
{
	LCD_write(cmd, CMD_MODE);
}

void LCD_char( unsigned char data )
{
	LCD_write(data, DATA_MODE);
}

void LCD_init (void)			/* LCD Initialize function */
{
	//LCD_DDR = 0xFF;			/* Make LCD port direction as o/p */ //4 bit mode, sets PORTD on MCU as output. Perhaps unnecessary in I2C since it's through SCL/SDA
	twi_init();
	
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

int main(void)
{
	LCD_init();			/* Initialization of LCD, twi_init() is called here */
    LCD_wake();			/* Wake function to test LCD functionality */
	LCD_str("Made by Vincent");
	LCD_cmd(0xC0);
	LCD_str("And Nick!");
	_delay_ms(500);
	LCD_clr();
	while(1);
}

