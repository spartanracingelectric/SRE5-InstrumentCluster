/*
 * uart_printf.h
 *
 * 
 *  Author: sean
 */ 


#ifndef UART_PRINTF_H_
#define UART_PRINTF_H_

#include <stdio.h>

void uart__printf(char *message);
void uart__print_num(uint8_t number); 
void uart__print_binary(uint8_t number);
void uart__print_can_id(uint16_t id); 

#endif /* UART_PRINTF_H_ */