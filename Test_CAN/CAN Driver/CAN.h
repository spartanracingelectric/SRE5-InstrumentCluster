/*
 * CAN.h
 *
 * Created: 1/30/2021 2:18:56 PM
 *  Author: Waylon
 */ 


#ifndef CAN_H_
#define CAN_H_

#include "spi.h"
#include "CAN_defs.h"
#include <stdint.h>
#include <stdbool.h>
#include "uart.h"
#include "uart_printf.h"



static int uart_putchar(char c, FILE *stream);
static FILE mystdout = FDEV_SETUP_STREAM(uart_putchar, NULL, _FDEV_SETUP_WRITE);
static int uart_putchar(char c, FILE *stream)
{
	if (c == '\n')
	uart_putchar('\r', stream);
	while(!(UCSR0A & (1<<UDRE0)));
	UDR0 = c;
	return 0;
}


uint8_t readRegister(uint8_t address)
{
	/* Send read instruction, address, and receive result */
	spiMasterChipSelect(1);
	spiMasterTRANSMIT(READ_INSTRUCTION);
	_delay_ms(10);
	spiMasterTRANSMIT(address);
	_delay_ms(10);
	uint8_t buffer = spiMasterTRANSMIT(0);
	_delay_ms(10);
	spiMasterChipSelect(0);
	return buffer;
}

void writeRegister(uint8_t address, uint8_t value)
{
	/* Send write instruction, address, and data */
	spiMasterChipSelect(1);
	_delay_ms(10);
	spiMasterTRANSMIT(WRITE_INSTRUCTION);
	_delay_ms(10);
	spiMasterTRANSMIT(address);
	_delay_ms(10);
	spiMasterTRANSMIT(value);
	_delay_ms(10);
	spiMasterChipSelect(0);
}

void changeBits(uint8_t address, uint8_t mask, uint8_t value)
{
	spiMasterChipSelect(1);
	spiMasterTRANSMIT(BITMODIFY_INSTRUCTION);
	_delay_ms(10);
	spiMasterTRANSMIT(address);
	_delay_ms(10);
	spiMasterTRANSMIT(mask);
	_delay_ms(10);
	spiMasterTRANSMIT(value);
	_delay_ms(10);
	spiMasterChipSelect(0);
	
}

void resetMCP2515()
{
	/* Send reset instruction */
	PORT_SPI &= ~(1<<PIN_SS);
	spiMasterTRANSMIT(RESET_INSTRUCTION);
	PORT_SPI |= (1<<PIN_SS);
	_delay_ms(10);
}

void setNormalMode()
{
	changeBits(CANCTRL, (0x7<<5), 0x0);
}

void setConfigurationMode()
{
	changeBits(CANCTRL, (0x7<<5), (0x4)<<5);
}

void setBitTiming(uint8_t rCNF1, uint8_t rCNF2, uint8_t rCNF3)
{
	writeRegister(CNF1, rCNF1);
	writeRegister(CNF2, rCNF2);
	changeBits(CNF3, 0x07, rCNF3);
}

void filters_off()
{
	changeBits(RXB0CTRL, (1<<RXM0), 0x20);
	changeBits(RXB0CTRL, (1<<RXM1), 0x40);
	changeBits(RXB1CTRL, (1<<RXM0), 0x20);
	changeBits(RXB1CTRL, (1<<RXM1), 0x40);
}

void rollover(bool status)
{
	if (status == true)
	{
		changeBits(RXB0CTRL, (1<<BUKT), 0x4);
	}
	else
	{
		changeBits(RXB0CTRL, (1<<BUKT), 0x0);
	}
}


bool mcp2515__init(void)
{
	/* Initialize SPI as a master device, on frequency < 10Mhz */
	spiMasterINIT();
	/* Send reset instruction */
	printf("\n\nCANCTRL = %x\n", readRegister(CANCTRL)>>5);
	resetMCP2515();
	setConfigurationMode();
	printf("CANCTRL AFTER = %x\n", readRegister(0x0F)>>5);
	printf("CANSTAT = %x\n\n", readRegister(0x0E)>>5);
	if ((readRegister(CANSTAT) >>5) != CONFIGURATION_MODE)
	{
		printf("Failed to enter configuration mode\n");
		return false;
	}else
		{
			printf("Successfully entered configuration mode\n");
		}
	setBitTiming(0x00, 0xF0, 0x86); //500kbps
	printf("Bit timing set successfully \n");
	filters_off();
	rollover(false);
	_delay_ms(100);
	setNormalMode();
	return true;
}



#endif /* CAN_H_ */