#include "spi.h"
#include <util/delay.h>

#ifndef SD_CARD_H_
#define SD_CARD_H_

//Port controls
#define SELECT()	PORTB &= (~SS)	//SS set to LOW
#define DESELECT()	PORTB |= SS

//SD card command definitions
#define CMD0		0x40			//Go Idle State
#define CMD8		0x40 + 8		//Send Interface Condition (checks card's version)

//SD card command arguments
#define CMD0_A		0x00000000
#define CMD8_A		0x0000001AA

//SD card command CRC
#define CMD0_C		0x94
#define CMD8_C		0x86

//Prototypes
void sd_powerUpSequence(void);
void sd_CMD(uint8_t, uint32_t, uint8_t);
void sd_init(void);
void sd_readR7(uint8_t *);
void sd_sendIfCond(uint8_t *);
uint8_t sd_readR1(void);
uint8_t sd_goIdleState(void);

//Res7's five bytes
uint8_t res7[5];
uint8_t *res7p;

/*
SD card power up sequence
Delays for 1ms and sends 80 clock cycles as per data sheet requirements
*/
void sd_powerUpSequence() {
	DESELECT();
	_delay_ms(1);
	for(uint8_t i = 0; i < 10; i++) {
		tx_spi(0xFF);
	}
	DESELECT();
	tx_spi(0xFF);
}

/*
Send a command, arguments, and crc to the SD card
*/
void sd_CMD(uint8_t cmd, uint32_t arg, uint8_t crc) {
	//transmit command to the SD card with the start and transmission bit
	tx_spi(cmd|0x40);
	
	//transmit argument one byte at a time
	tx_spi((uint8_t)(arg >> 24));	//Bits [31 - 24]
	tx_spi((uint8_t)(arg >> 16));	//Bits [23 - 16]
	tx_spi((uint8_t)(arg >> 8));	//Bits [15 - 8]
	tx_spi((uint8_t)(arg));			//Bits [7 - 0]

	//transmit crc with end bit
	tx_spi(crc|0x01);
}

/*
Reads and returns the response from the SD card after a command is sent
R1 is one byte, MSB (bit 7) is always 0, each bit is a flag for a different state/error
Bit 6 -> 0 in order:
Parameter error
Address error
Erase sequence error
Com crc error
Illegal command
Erase reset
In idle state
*/
uint8_t sd_readR1() {
	uint8_t count = 0, out;
	
	//poll until data is received
	while((out = rx_spi()) == 0xFF) {
		count++;

		//if no data was received for 8 bytes, break
		if(count > 8) break;
	}

	return out;
}

//Sequence to execute CMD0
uint8_t sd_goIdleState() {
	//Select SD card
	rx_spi();
	SELECT();
	rx_spi();

	//Send CMD0
	sd_CMD(CMD0, CMD0_A, CMD0_C);

	//Read response
	uint8_t r1 =  sd_readR1();

	//Deselect chip
	rx_spi();
	DESELECT();
	rx_spi()
}

void sd_readR7(uint8_t *res) {
	//the first byte of R7 is identical to R1
	res[0] = sd_readR1();

	//return if there is an error in R1
	if(res[0] > 1) return;

	//read the remaining bytes
	res[1] = rx_spi();
	res[2] = rx_spi();
	res[3] = rx_spi();
	res[4] = rx_spi();
}

void sd_sendIfCond(uint8_t *res) {
	//Select SD Card
	rx_spi();
	SELECT();
	rx_spi();

	//Send CMD8
	sd_CMD(CMD8, CMD8_A, CMD8_C);

	//Read response
	sd_readR7(res);

	//Deselect chip
	rx_spi();
	DESELECT();
	rx_spi();
}


void sd_init() {
	spi_init();
	sd_powerUpSequence();
	sd_goIdleState();
}