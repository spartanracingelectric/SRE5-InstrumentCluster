/*
 *  mcp2515.c
 *
 * 
 *  Author: sean
 */ 
#define F_CPU 16000000UL
#include "mcp2515.h"

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

#include "mcp2515_defs.h"
#include "spi.h"

#include <avr/sfr_defs.h>

bool mcp2515_init(uint16_t baudrate_kbps)
{	
	// set chip select, PortB Pin2
	PORTB |= (1 << PINB2); 
	DDRB |= (1 << PINB2); 
	
	// set interrupt pin, PortD Pin2
	DDRD &= ~(1 << PIND2); 
	PORTD |= (1 << PIND2); 
	
	mcp2515_software_reset(); 
	
	// 16MHz = 62.5ns/clk Fosc = Tosc = 62.5ns
	// 500kHz = 2000ns = 2us
	// 250kHz = 4000ns = 4us
	// 125kHz = 8000ns = 8us
	
	// const uint32_t oscillator_hz = 16000000;
 	// for 16MHz and default value or BRP is 0, the default Tq = 125ns 
 	// maximum NBT = 1 + 8 + 8 + 8 = 25
 	// minimum NBT = 1 + 1 + 1 + 2 = 5

 	/* requirements of time segments
 	   PropSeg + PS1 >= PS2
 	   PropSeg + PS1 >= Tdelay
 	   PS2 > SJW
 	 */ 
	/*
	Tq = 2 x (BRP[5:0] + 1) / Fosc
	uint8_t baud_divider = oscillator_hz / (baudrate_kbps * 1000) / 2;
	uint8_t NBT;
 	uint8_t BRP = 0, PROPSEG = 0, PS1 = 0, PS2 = 3; 
 	for (NBT = 24; NBT > 0; NBT -= 2) {
 		if (0 == (baud_divider % NBT)) {
 			BRP = baud_divider / NBT - 1;
			NBT--; 
			PS1 = (NBT / 3) - 1;
			PROPSEG = NBT - (PS1 + 1) - (PS2 + 1) - 1; 
			break; 
		} 
	}
	mcp2515_write_register(CNF1, BRP);
	mcp2515_write_register(CNF2, PROPSEG | (PS1 << 3) | (1 << 7));
	mcp2515_write_register(CNF3, PS2);
	*/

	mcp2515_cs_reset(); 
	spi_exchange_byte(SPI_WRITE);
	spi_exchange_byte(CNF3);
	
	/* Use pre-calculate fixed value for now, SJW = 1, sample point at 87.5%
		fixed speed 500kHz, input speed will be ignored 
	 */
	spi_exchange_byte(0x01);
	spi_exchange_byte(0xB5);
	spi_exchange_byte(0x00);
	
	// enable interrupt
	spi_exchange_byte((1 << RX1IE) | (1 << RX0IE));
	mcp2515_cs_set(); 
	
	printf("CNF3 = %x\n",  mcp2515_read_register(CNF3)); 
	if ( mcp2515_read_register(CNF3) != 1) {
		printf("CNF3 = %x\n",  mcp2515_read_register(CNF3)); 
		return false;
	}

	mcp2515_write_register(BFPCTRL, 0x00);
	mcp2515_write_register(TXRTSCTRL, 0x00);

	mcp2515_turn_off_filter_receive_all_message();

	// printf("RXB0CTRL = %02hhX\n",  mcp2515_read_register(RXB0CTRL)); 

	// put mcp2515 in normal mode
	mcp2515_set_mode(normal_mode); 
	return true; 
}

void mcp2515_cs_set(void)
{
	PORTB |= (1 << PINB2); 
}

void mcp2515_cs_reset(void)
{
	PORTB &= ~(1 << PINB2);
}

void mcp2515_write_register(uint8_t address, uint8_t data)
{
	mcp2515_cs_reset(); 
	
	spi_exchange_byte(SPI_WRITE); 
	
	spi_exchange_byte(address); 
	spi_exchange_byte(data);
	
	mcp2515_cs_set(); 
}

void mcp2515_bit_modify(uint8_t address, uint8_t bitmask, uint8_t data)
{
	mcp2515_cs_reset();
	
	spi_exchange_byte(SPI_BIT_MODIFY);
	
	spi_exchange_byte(address);
	spi_exchange_byte(bitmask);
	spi_exchange_byte(data);
	
	mcp2515_cs_set();
}

uint8_t mcp2515_read_register(uint8_t address)
{
	uint8_t data; 
	mcp2515_cs_reset(); 
	
	spi_exchange_byte(SPI_READ);
	spi_exchange_byte(address);
	data = spi_exchange_byte(0xFF);
	
	mcp2515_cs_set(); 
	
	return data; 
}

uint8_t mcp2515_read_status(void)
{
	uint8_t mcp2515_status;
	
	mcp2515_cs_reset();
	
	spi_exchange_byte(SPI_READ_STATUS);
	mcp2515_status = spi_exchange_byte(0xFF);
	
	mcp2515_cs_set();
	
	return mcp2515_status;
}

bool mcp2515_set_mode(mcp2515_mode_e mode)
{
	bool mode_set_successfully = false; 
	uint8_t status;
	mcp2515_write_register(CANCTRL, mode); 
	status = mcp2515_read_status();
	if (mode == (status & 0xF0)) {
		mode_set_successfully = true; 
	}
	return mode_set_successfully; 
}

void mcp2515_software_reset(void)
{
	/* Using SPI instructions reset internal registers to default state,
	 * set MCP2515 to Configuration mode. 
	 */ 
	mcp2515_cs_reset();
	
	spi_exchange_byte(SPI_RESET);
	
	mcp2515_cs_set(); 
	_delay_us(10); 
}

void mcp2515_turn_off_filter_receive_all_message(void)
{
	// turn off filter for both receive buffer
	mcp2515_write_register(RXB0CTRL, 0x60);
	mcp2515_write_register(RXB1CTRL, 0x60);
}

bool mcp2515_tx(can_message_s *msg_ptr)
{
	uint8_t buffer_index; 
	uint8_t status = mcp2515_read_status(); 
	
	// check receive buffer
	if (!(status & (1 << 2))) {
		buffer_index = 0x00; 
	} else if (!(status & (1 << 4))) {
		buffer_index = 0x02;
	} else if (!(status & (1 << 6))) {
		buffer_index = 0x04;
	} else {
		return false; 
	}

	mcp2515_cs_reset();
	
	spi_exchange_byte(SPI_LOAD_TX_BUFFER | buffer_index); 
	
	spi_exchange_byte(msg_ptr->id >> 3); 
	spi_exchange_byte(msg_ptr->id << 5);
	spi_exchange_byte(0); 
	spi_exchange_byte(0);
		
	spi_exchange_byte(msg_ptr->data_length & 0x0F); 
	
	for (int i = 0; i < msg_ptr->data_length; i++) {
		spi_exchange_byte(msg_ptr->data[i]); 
	}
	
	mcp2515_cs_set(); 
	_delay_us(1);
	
	mcp2515_cs_reset();
	
	buffer_index = (buffer_index == 0) ? 1 : buffer_index;
	spi_exchange_byte(SPI_RTS | buffer_index);
	
	mcp2515_cs_set();

	return true; 
}

bool mcp2515_rx(can_message_s *msg_ptr) 
{

	uint8_t buffer_index;
	uint8_t status = mcp2515_read_rx_buffer_status(); 

	// check receive buffer
	if (status & (1 << 6)) {
		buffer_index = 0x90; // buffer 0
	} else if (status & (1 << 7)) {
		buffer_index = 0x94; // buffer 1
	} else {
		return false;
	}

	mcp2515_cs_reset();
	spi_exchange_byte(buffer_index); 
	msg_ptr->id = (uint16_t) spi_exchange_byte(0xFF) << 3;
	msg_ptr->id |= spi_exchange_byte(0xFF) >> 5;

	// dump useless info
	spi_exchange_byte(0xFF);
	spi_exchange_byte(0xFF); 
	
	msg_ptr->data_length = spi_exchange_byte(0xFF) & 0x0F; 

	for (int i = 0; i < msg_ptr->data_length; i++) {
		msg_ptr->data[i] = spi_exchange_byte(0xFF); 
	}
	mcp2515_cs_set();

	if (status & (1 << 6)) {
		mcp2515_bit_modify(CANINTF, (1 << RX0IF), 0); 
	} 
	
	if (status & (1 << 7)) {
		mcp2515_bit_modify(CANINTF, (1 << RX1IF), 0); 
	}
	
	return true; 
}

bool mcp2515_is_there_new_message(void)
{
	if (!(PORTD & (1 << PIND2))) {
		return true; 
	}
	return false; 
}


uint8_t mcp2515_read_rx_buffer_status(void)
{
	uint8_t buffer_status;
	
	mcp2515_cs_reset();
	
	spi_exchange_byte(SPI_RX_STATUS);
	buffer_status = spi_exchange_byte(0xFF);
	
	mcp2515_cs_set();
	
	return buffer_status;
}