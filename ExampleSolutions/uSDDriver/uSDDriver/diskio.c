/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for Petit FatFs (C)ChaN, 2014      */
/*-----------------------------------------------------------------------*/

#include "diskio.h"

#define CS_LOW()	PORTB &= ~(1<<CS)	/* Set CS low */
#define	CS_HIGH()	PORTB |=  (1<<CS)	/* Set CS high */
#define	IS_CS_LOW	!(PINB & (1<<CS))	/* Test if CS is low */

/* Definitions for MMC/SDC command */
#define CMD0	(0x40+0)	/* GO_IDLE_STATE */
#define CMD1	(0x40+1)	/* SEND_OP_COND (MMC) */
#define	ACMD41	(0xC0+41)	/* SEND_OP_COND (SDC) */
#define CMD8	(0x40+8)	/* SEND_IF_COND */
#define CMD16	(0x40+16)	/* SET_BLOCKLEN */
#define CMD17	(0x40+17)	/* READ_SINGLE_BLOCK */
#define CMD24	(0x40+24)	/* WRITE_BLOCK */
#define CMD55	(0x40+55)	/* APP_CMD */
#define CMD58	(0x40+58)	/* READ_OCR */

/* Card type flags (CardType) */
#define CT_MMC				0x01	/* MMC version 3 */
#define CT_SD1				0x02	/* SD version 1 */
#define CT_SD2				0x04	/* SD version 2+ */
#define CT_BLOCK			0x08	/* Block addressing */

static BYTE CardType;

/*-----------------------------------------------------------------------*/
/* Send a command packet to MMC                                          */
/*-----------------------------------------------------------------------*/

unsigned char rcv_spi() {
	return spi_tranceiver(0xFF);
}

static BYTE send_cmd (
	BYTE cmd,		/* 1st byte (Start + Index) */
	DWORD arg		/* Argument (32 bits) */
)
{
	BYTE n, res;

	if (cmd & 0x80) {	/* ACMD<n> is the command sequence of CMD55-CMD<n>, checks if cmd is ACMD */
		cmd &= 0x7F;
		res = send_cmd(CMD55, 0);
		if (res > 1) return res;
	}

	/* Select the card */
	CS_HIGH();
	rcv_spi();
	CS_LOW();
	rcv_spi();


	/* Send a command/argument packet */
	spi_tranceiver(cmd);						/* Start + Command index */
	spi_tranceiver((BYTE)(arg >> 24));		/* Argument[31..24] */
	spi_tranceiver((BYTE)(arg >> 16));		/* Argument[23..16] */
	spi_tranceiver((BYTE)(arg >> 8));			/* Argument[15..8] */
	spi_tranceiver((BYTE)arg);				/* Argument[7..0] */
	
	/* Send a cyclic redundancy check packet */
	n = 0x01;							/* Dummy CRC + Stop */
	if (cmd == CMD0) n = 0x95;			/* Valid CRC for CMD0(0) */
	if (cmd == CMD8) n = 0x87;			/* Valid CRC for CMD8(0x1AA) */
	spi_tranceiver(n);

	/* Receive a command response, R resp. */
	n = 10;								/* Wait for a valid response in timeout of 10 attempts */
	do {
		res = rcv_spi();
	} while ((res & 0x80) && --n); //While MSB of res non-zero

	return res;			/* Return with the response value */
}

/*-----------------------------------------------------------------------*/
/* Initialize Disk Drive                                                 */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (void)
{
	BYTE n, cmd, ty, ocr[4]; //OCR bit width is 32
	UINT tmr;

#if PF_USE_WRITE
	//if (CardType != 0 && IS_CS_LOW) disk_writep(0, 0);	/* Finalize write process if it is in progress */
#endif

	spi_init();		/* Initialize ports to control MMC */
	CS_HIGH();
	for (n = 10; n; n--) rcv_spi();	/* 80 dummy clocks with CS=H */

	ty = 0;
	if (send_cmd(CMD0, 0) == 1) {							// If R1 idle state bit true, GO_IDLE_STATE
		if (send_cmd(CMD8, 0x1AA) == 1) {					// for SDv2/SDHC/SDXC
			for (n = 0; n < 4; n++) ocr[n] = rcv_spi();		// Receive and store trailing R7 response data, store to ocr[] array
			if (ocr[2] == 0x01 && ocr[3] == 0xAA) {			/* The card can work at vdd range of 2.7-3.6V */
				for (tmr = 10000; tmr && send_cmd(ACMD41, 1UL << 30); tmr--) _delay_us(100);	/* Wait for leaving idle state (ACMD41 with HCS bit) */
				if (tmr && send_cmd(CMD58, 0) == 0) {		/* Check CCS bit in the OCR */
					for (n = 0; n < 4; n++) ocr[n] = rcv_spi();
					ty = (ocr[0] & 0x40) ? CT_SD2 | CT_BLOCK : CT_SD2;	/* SDv2 (HC or SC) */
				}
			}
		}
		else {							/* SDv1 or MMCv3 */
			if (send_cmd(ACMD41, 0) <= 1) {
				ty = CT_SD1; cmd = ACMD41;	/* SDv1 */
			}
			else {
				ty = CT_MMC; cmd = CMD1;	/* MMCv3 */
			}
			for (tmr = 10000; tmr && send_cmd(cmd, 0); tmr--) _delay_us(100);	/* Wait for leaving idle state */
			if (!tmr || send_cmd(CMD16, 512) != 0) {	/* Set R/W block length to 512 */
				ty = 0;
			}
		}
	}
	CardType = ty;
	CS_HIGH();
	rcv_spi();

	return ty ? 0 : STA_NOINIT; //If 
}


/*-----------------------------------------------------------------------*/
/* Read Partial Sector                                                   */
/*-----------------------------------------------------------------------*/
/*
DRESULT disk_readp (
	BYTE* buff,		// Pointer to the destination object
	DWORD sector,	// Sector number (LBA)
	UINT offset,	// Offset in the sector
	UINT count		// Byte count (bit15:destination)
)
{
	DRESULT res;

	// Put your code here

	return res;
}
*/


/*-----------------------------------------------------------------------*/
/* Write Partial Sector                                                  */
/*-----------------------------------------------------------------------*/
/*
DRESULT disk_writep (
	BYTE* buff,		// Pointer to the data to be written, NULL:Initiate/Finalize write operation
	DWORD sc		// Sector number (LBA) or Number of bytes to send
)
{
	DRESULT res;


	if (!buff) {
		if (sc) {

			// Initiate write process

		} else {

			// Finalize write process

		}
	} else {

		// Send data to the disk

	}

	return res;
}
*/

