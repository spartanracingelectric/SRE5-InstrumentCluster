/*-----------------------------------------------------------------------
/  PFF - Low level disk interface modlue include file    (C)ChaN, 2014
/-----------------------------------------------------------------------*/

#ifndef _DISKIO_DEFINED
#define _DISKIO_DEFINED

#define F_CPU 16000000UL //Change MCU speed here, delay.h dependent

#ifdef __cplusplus
extern "C" {
#endif

#include "pff.h"
#include "spi.h"
#include "1602lcd.h"

/* Status of Disk Functions */
typedef BYTE	DSTATUS;


/* Results of Disk Functions */
typedef enum {
	RES_OK = 0,		/* 0: Function succeeded */
	RES_ERROR,		/* 1: Disk error */
	RES_NOTRDY,		/* 2: Not ready */
	RES_PARERR		/* 3: Invalid parameter */
} DRESULT;

/*------------------------*/
/* Disk control functions */
DSTATUS disk_initialize (void);
DRESULT disk_readp (BYTE* buff, DWORD sector, UINT offset, UINT count);
DRESULT disk_writep (const BYTE* buff, DWORD sc);
void disk_display_init_info(void); //Include in loops for continuous checking. Displays to LCD

#define STA_NOINIT		0x01	/* Drive not initialized */
#define STA_NODISK		0x02	/* No medium in the drive */

#ifdef __cplusplus
}
#endif

#endif	/* _DISKIO_DEFINED */
