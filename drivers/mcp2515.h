/*
 *  mcp2515.h
 *
 *	
 *  Author: sean
 */ 
#pragma once

#include <stdint.h>
#include <stdbool.h>

typedef struct {
	uint16_t id;
	uint8_t data_length;
	uint8_t data[8];
}can_message_s;

typedef enum {
	normal_mode = 0x00,
	sleep_mode = 0x20,
	loopback_mode = 0x40,
	listen_only_mode = 0x60,
	configuration_mode = 0x80
} mcp2515_mode_e;

bool mcp2515_init(uint16_t baudrate_kbps);
void mcp2515_cs_set(void); 
void mcp2515_cs_reset(void); 
void mcp2515_software_reset(void); 
void mcp2515_write_register(uint8_t address, uint8_t command); 
void mcp2515_bit_modify(uint8_t address, uint8_t bitmask, uint8_t data); 
void mcp2515_turn_off_filter_receive_all_message(void); 
uint8_t mcp2515_read_status(void); 
uint8_t mcp2515_read_register(uint8_t address); 
uint8_t mcp2515_read_rx_buffer_status(void); 
bool mcp2515_set_mode(mcp2515_mode_e mode); 
bool mcp2515_tx(can_message_s *msg_ptr);  
bool mcp2515_rx(can_message_s *msg_ptr);
bool mcp2515_is_there_new_message(void); 



