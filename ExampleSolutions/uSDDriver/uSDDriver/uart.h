#ifndef UART_H_
#define UART_H_

#include "conf.h"
#include <avr/io.h>
#include <stdint.h>
#include <stdbool.h>

void uart__init(uint32_t baud_rate);
uint8_t uart__receive(void);
bool uart__polled_put(uint8_t data);

#endif /* UART_H_ */