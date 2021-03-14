/*
 *  spi.h
 *
 * 
 *  Author: sean
 */ 
#pragma once

#include <stdint.h>

void spi_init(void); 

uint8_t spi_exchange_byte(uint8_t byte_to_transmit);
