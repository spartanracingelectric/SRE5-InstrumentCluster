#ifndef I2C_H_
#define I2C_H_

#include <util/twi.h>

//I2C addresses
#define LCD_ADDRESS		0x70
#define LED_BAR_1		0x72
#define LED_BAR_2		0x74
#define LED_INDICATORS	0x76

void twi_init(void);
void twi_start(uint8_t i2c_address);
void twi_write(uint8_t data);
void twi_stop(void);

void error()
{
}

void twi_init(void)
{
	PRR &= ~(1<<PRTWI); //The PRTWI bit in Section 9.10 ?Minimizing Power Consumption" on page 36 must be written to zero to enable the 2-wire serial interface
	TWCR &= ~(1<<TWIE); // If the TWIE bit is cleared, the application must poll the TWINT flag in order to detect actions on the TWI bus
	TWSR &= ~(1<<TWPS0)|(1<<TWPS1); //Sets prescaler value to 1
	TWBR = 72; //Sets bit rate in TWBR; 72 = 100khz bit rate; 12 =400khz bit rate; calcluate with TWBR_val (((F_CPU / F_SCL) - 16 ) / (2*Prescaler))
}

void twi_start(uint8_t i2c_address)
{
	TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);	//Send START condition
	while (!(TWCR & (1<<TWINT)));	//Wait for TWINT flag set. This indicates that the START condition has been transmitted
	if ((TWSR & 0xF8) != TW_START) //Check value of TWI status register. Mask prescaler bits. If status different from START go to ERROR
	error();
	TWDR = (i2c_address << 1) | TW_WRITE;; //Sets address/data register to slave address + write
	TWCR = (1<<TWINT) | (1<<TWEN); //Clear TWINT bit in TWCR to start transmission of address
	while (!(TWCR & (1<<TWINT))); //Wait for TWINT flag set. This indicates that the SLA+W has been transmitted, and ACK/NACK has been received.
	if ((TWSR & 0xF8) != TW_MT_SLA_ACK) //Check value of TWI status register. Mask prescaler bits. If status different from TW_MT_SLA_ACK go to ERROR
	error();
}

void twi_write(uint8_t data)
{
	TWDR = data;  //Load DATA into TWDR register
	TWCR = (1<<TWINT) | (1<<TWEN);  //Clear TWINT bit in TWCR to start transmission of data
	while(!(TWCR & (1<<TWINT)));  //Wait for TWINT flag set. This indicates that the DATA has been transmitted, and ACK/NACK has been received
	if ((TWSR & 0xF8)!= TW_MT_DATA_ACK)  //Check value of TWI status register. Mask prescaler bits. If status different from TW_MT_DATA_ACK go to ERROR
	error();
}

void twi_stop(void)
{
	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO);  //Transmit STOP condition
}
#endif /* I2C_H_ */