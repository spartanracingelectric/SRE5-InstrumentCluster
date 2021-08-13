/*
 * i2c.h
 *
 * Created: 10/10/2020 2:23:39 PM
 *  Author: wayne
 */ 


#ifndef I2C_H_
#define I2C_H_

//I2C ADDRESSES
static const uint8_t LCD_ADDRESS = 0x38;
static const uint8_t LED_BAR_1 = 0x3A;
static const uint8_t LED_BAR_2 = 0x39;
static const uint8_t LED_INDICATORS = 0x3B;

//I2C INSTRUCTIONS
//Initialize I2C Communication
void i2c__init(void);
//Begin transmitting i2c message to address in the argument
void i2c__start(uint8_t i2c_address);
//Write data to i2c message
void i2c__write(uint8_t data);
//Stop i2c transmission to the address specified in i2c__start
void i2c__stop(void);

#endif /* I2C_H_ */