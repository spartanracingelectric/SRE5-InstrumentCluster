#ifndef UART_PRINTF_H_
#define UART_PRINTF_H_

#include "conf.h"
#include <stdio.h>
#include <string.h>
#include "uart.h"
#include "diskio.h"

/* Codes for different pff functions for uart__print_disk_error */
#define MOUNT 0
#define OPEN 1
#define READ 2

void uart__printf(char *message); /* Prints string/char array */
void uart__print_new_line(); /* New CRLF line (\r\n) */
void uart__print_welcome(); /* Welcome graphic. Optional. */
void uart__print_hex(unsigned char data); /* Byte in hex representation (0x??), for debug */
void uart__print_disk_info(void); /* SD/MMC disk info */
void uart__print_disk_error(unsigned char res, unsigned char type); 
/* res: FRESULT argument that indicates error		*
 * type: pff function type (MOUNT -> pf_mount, etc) */

#endif /* UART_PRINTF_H_ */