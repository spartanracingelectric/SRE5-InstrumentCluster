#include "uart_printf.h"

/* For disk_display_init_info() */
DSTATUS stat;


void uart__printf(char *message)
{
	const size_t message_length = strlen(message); 
	for(size_t i = 0; i < message_length; i++)
	{
		uart__polled_put(message[i]); 
	}
}


void uart__print_new_line() {
	uart__polled_put('\r');
	uart__polled_put('\n');
}


void uart__print_welcome() {
	char r1[39] = "___       __     _         __ ___ _  _";
	char r2[40] = " |  |\\ | (_     /  |  | | (_   | |_ |_)";
	char r3[40] = "_|_ | \\| __) o  \\_ |_ |_| __)  | |_ | \\";

	for(int i = 0; i < 39; i++)
		uart__polled_put('#');
	uart__print_new_line();
	uart__printf(r1);
	uart__print_new_line();
	uart__printf(r2);
	uart__print_new_line();
	uart__printf(r3);
	uart__print_new_line();
	uart__print_new_line();
	for(int i = 0; i < 39; i++)
		uart__polled_put('#');
	uart__print_new_line();
}


void uart__print_hex(unsigned char data) {
	unsigned char upperNibble, lowerNibble;
	upperNibble = (data & 0xF0) >> 4;
	if (upperNibble <= 9) {
		upperNibble += 48;
	}
	else {
		upperNibble += 55;
	}
	lowerNibble = data & 0x0F;
	if (lowerNibble <= 9) {
		lowerNibble += 48;
	}
	else {
		lowerNibble += 55;
	}
	uart__polled_put('0');
	uart__polled_put('x');
	uart__polled_put(upperNibble);
	uart__polled_put(lowerNibble);
}


void uart__print_disk_info(void) {
	stat = disk_initialize();
	if (!stat) { //There is no error and flag indicating change needed
		uart__printf("*** DISK TYPE: ");
		if (disk_get_card_type() == CT_MMC) {
			uart__printf("MMCv3");
		}
		else if (disk_get_card_type() == CT_SD1) {
			uart__printf("SDSC");
		}
		else if (disk_get_card_type() == CT_SD2) {
			uart__printf("SDHC+ (BYTE MODE)");
		}
		else if (disk_get_card_type() == (CT_SD2 | CT_BLOCK)) {
			uart__printf("SDHC+ (BLOCK MODE)");
		}
		else {
			uart__printf("UNKNOWN, CODE = ");
			uart__print_hex(disk_get_card_type());
		}
		uart__print_new_line();
	}
	else { //There is an error
		uart__printf("*** COULD NOT PRINT DISK INFO");
		uart__print_new_line();
	}
}


void uart__print_disk_error(unsigned char res, unsigned char type) {
	switch(type) {
		case MOUNT: { //Switch of pff functions
			if (!res) { //No error, res == FR_OK
				uart__printf("*** MOUNT SUCCESS!");
			}
			else { //Non-zero means error
				uart__printf("*** MOUNT FAIL...");
			}
			break;
		}
		case OPEN: {
			if (!res) { //No error, res == FR_OK
				uart__printf("*** FILE ");
				uart__printf(FILE_NAME);
				uart__printf(" OPEN SUCCESS!");
			}
			else { //Non-zero means error
				uart__printf("*** FILE ");
				uart__printf(FILE_NAME);
				uart__printf(" OPEN FAIL...");
			}
			break;
		}
		case READ: {
			if (!res) { //No error, res == FR_OK
				uart__printf("*** READ SUCCESS!");
			}
			else { //Non-zero means error
				uart__printf("*** READ FAIL...");
			}
			break;
		}
	}
	uart__print_new_line();
	
	if (res) {
		uart__printf("Err: ");
		switch(res) { //Switch of all FRESULT enumerators, except FR_OK
			case FR_DISK_ERR: {
				uart__printf("FR_DISK_ERR (1)");
				break;
			}
			case FR_NOT_READY: {
				uart__printf("FR_NOT_READY (2)");
				break;
			}
			case FR_NO_FILE: {
				uart__printf("FR_NO_FILE (3)");
				break;
			}
			case FR_NOT_OPENED: {
				uart__printf("FR_NOT_OPENED (4)");
				break;
			}
			case FR_NOT_ENABLED: {
				uart__printf("FR_NOT_ENABLED (5)");
				break;
			}
			case FR_NO_FILESYSTEM: {
				uart__printf("FR_NO_FILESYSTEM (6)");
				break;
			}
		}
		uart__print_new_line();
	}
}