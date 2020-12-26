/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for Petit FatFs (C)ChaN, 2014      */
/*-----------------------------------------------------------------------*/

#include "diskio.h"

#define CS_LOW()	PORTB &= ~(1<<CS)	/* Set CS low */
#define	CS_HIGH()	PORTB |=  (1<<CS)	/* Set CS high */
#define	IS_CS_LOW	!(PINB & (1<<CS))	/* Test if CS is low */
#define	FORWARD(d)	uart__polled_put(d)		/* Data streaming function (console out) */
//#define	FORWARD_STR(d)	uart__printf(d)		/* Data streaming function (console out) */

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

/*--------------------------------------------------------------------------*/
/* INITIALIZE DISK DRIVE                                                    */
/*																			*/
/* Initialization Flowchart: http://elm-chan.org/docs/mmc/i/sdinit.png      */
/* DSTATUS: byte for error status -> 0 (pass) or 1 (STA_NOINIT, fail)       */
/* BYTE n: For ocr[4], represents counter for # of nibbles in OCR width     */
/* BYTE cmd: SD CMDs are assigned to it, used to differentiate SDv1 & MMCv3 */
/* BYTE ty: Represents SD card type. 0 end val = missing SD ver., init fail */
/* BYTE ocr[4]: Stores OCR of R3 & R7 responses, 32 bits wide				*/
/* UINT tmr: Timer var														*/
/*--------------------------------------------------------------------------*/
DSTATUS disk_initialize (void)
{
	BYTE n, cmd, ty, ocr[4];
	UINT tmr;

#if PF_USE_WRITE
	if (CardType != 0 && IS_CS_LOW) disk_writep(0, 0);	/* Finalize write process if it is in progress */
#endif

	spi_init();		/* Initialize ports to control MMC */
	CS_HIGH();
	//for (n = 100; n; n--) _delay_us(100);	/* 10ms delay */
	for (n = 10; n; n--) rcv_spi();	/* 80 dummy clocks with CS=H */

	ty = 0;
	if (send_cmd(CMD0, 0) == 1) {							// If R1 idle state bit true, GO_IDLE_STATE
		if (send_cmd(CMD8, 0x1AA) == 1) {					// for SDv2/SDHC/SDXC
			for (n = 0; n < 4; n++) ocr[n] = rcv_spi();		// Receive and store trailing R7 response data, store to ocr[] array. R7 is 32 bits long
			if (ocr[2] == 0x01 && ocr[3] == 0xAA) {			/* The card can work at vdd range of 2.7-3.6V */
				for (tmr = 10000; tmr && send_cmd(ACMD41, 1UL << 30); tmr--) _delay_us(100);	/* Wait for leaving idle state (ACMD41 with HCS bit) */
				if (tmr && send_cmd(CMD58, 0) == 0) {		/* Check CCS bit in the OCR */
					for (n = 0; n < 4; n++) ocr[n] = rcv_spi(); // Receive and store trailing R3 response data, store to ocr[] array. R3 is 32 bits long
					ty = (ocr[0] & 0x40) ? CT_SD2 | CT_BLOCK : CT_SD2;	/* SDv2 (HC+) */
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
			for (tmr = 10000; tmr && send_cmd(cmd, 0); tmr--) _delay_us(100);	/* Wait for leaving idle state, min. timeout of 1s. Ping every 100us. */
			if (!tmr || send_cmd(CMD16, 512) != 0) {	/* Set R/W block length to 512 */
				ty = 0;
			}
		}
	}
	CardType = ty;
	CS_HIGH();
	rcv_spi();

	/* If any non-zero, card has initialized to some config */
	/*  already and return 0. Otherwise, return STA_NOINIT  */
	return ty ? 0 : STA_NOINIT; }

/*-----------------------------------------------------------------------*/
/* Read Partial Sector                                                   */
/*-----------------------------------------------------------------------*/

DRESULT disk_readp (
	BYTE* buff,		// Pointer to the destination object
	DWORD sector,	// Sector number (LBA)
	UINT offset,	// Offset in the sector
	UINT count		// Byte count (bit15:destination)
)
{
	DRESULT res;
	BYTE rc;
	UINT bc;
	
	if (!(CardType & CT_BLOCK)) sector *= 512;	/* If not SDHC+ BLOCK, convert to byte address */

	res = RES_ERROR; //Default state, hard err occurred during read and couldn't recover, return RES_ERROR (1). Cleared with successful read.
	if (send_cmd(CMD17, sector) == 0) {	/* READ_SINGLE_BLOCK */

		bc = 40000;	/* Time counter */
		do {				/* Wait for data block */
			rc = rcv_spi();
		} while (rc == 0xFF && --bc);

		if (rc == 0xFE) {	/* A data block arrived */

			bc = 514 - offset - count;	/* Number of trailing bytes to skip */

			/* Skip leading bytes in the sector */
			while (offset--) rcv_spi();

			/* Receive a part of the sector */
			if (buff) {	/* Store data to the memory */
				do {
					*buff++ = rcv_spi();
				} while (--count);
			}
			else {	/* Forward data to the outgoing stream */
				do {
					FORWARD(rcv_spi());
				} while (--count);
			}

			/* Skip trailing bytes in the sector and block CRC */
			do rcv_spi(); while (--bc);

			res = RES_OK; //Function succeeded, return RES_OK (0)
		}
	}

	CS_HIGH();
	rcv_spi();


	return res;
}



/*-----------------------------------------------------------------------*/
/* Write Partial Sector                                                  */
/*-----------------------------------------------------------------------*/

#if PF_USE_WRITE
DRESULT disk_writep (
	const BYTE* buff,		// Pointer to the data to be written, NULL:Initiate/Finalize write operation
	DWORD sc		// Sector number (LBA) or Number of bytes to send
)
{
	DRESULT res;
	UINT bc;
	static UINT wc;	/* Sector write counter */

	res = RES_ERROR;

	if (buff) {		/* Send data bytes */
		bc = sc;
		while (bc && wc) {		/* Send data bytes to the card */
			spi_tranceiver(*buff++);
			wc--; bc--;
		}
		res = RES_OK;
	}
	else {
		if (sc) {	/* Initiate sector write process */
			if (!(CardType & CT_BLOCK)) sc *= 512;	/* Convert to byte address if needed */
			if (send_cmd(CMD24, sc) == 0) {			/* WRITE_SINGLE_BLOCK */
				spi_tranceiver(0xFF); spi_tranceiver(0xFE);		/* Data block header */
				wc = 512;							/* Set byte counter */
				res = RES_OK;
			}
		}
		else {	/* Finalize sector write process */
			bc = wc + 2;
			while (bc--) spi_tranceiver(0);	/* Fill left bytes and CRC with zeros */
			if ((rcv_spi() & 0x1F) == 0x05) {	/* Receive data resp and wait for end of write process in timeout of 500ms */
				for (bc = 5000; rcv_spi() != 0xFF && bc; bc--) {	/* Wait for ready */
					_delay_us(100);
				}
				if (bc) res = RES_OK;
			}
			CS_HIGH();
			rcv_spi();
		}
	}

	return res;
}
#endif /* PF_USE_WRITE */


/*-----------------------------------------------------------------------*/
/* Get MMC/SD Card Type			                                         */
/*-----------------------------------------------------------------------*/

BYTE disk_get_card_type() {
	return CardType; //Returns defined values
}
