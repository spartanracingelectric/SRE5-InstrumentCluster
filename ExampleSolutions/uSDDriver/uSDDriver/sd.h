#ifndef SD_H_
#define SD_H_

/************************************/
/*		SD SIMPLIFIED FUNCTIONS		*/
/************************************/

#include "conf.h"
#include "pff.h"
#include "uart_printf.h"

/* Usage:								    *
*  buff is typically an array/string input  *
*  btr/btw is typically sizeof(buff)		*
*  Example: sd__read(str, sizeof(str));		*/

extern int sdOffset;

void sd__mount(FATFS* fs);
void sd__open(const char* path);
void sd__read_br(void* buff, UINT btr, UINT* br); //br is passed as ptr, then changed. # of bytes to read = sizeof(buffer). 
void sd__read(void* buff, UINT btr); //br not needed (local temp var passed). Builds off of sd__read_br
void sd__write_bw(const void* buff, UINT btw, UINT* bw); //bw is passed as ptr, then changed. # of bytes to write = sizeof(buffer).
void sd__write(const void* buff, UINT btw); //bw not needed (local temp var passed). Builds off of sd__write_bw
void sd__new_entry(const void* buff, UINT btw, UINT* bw);


#endif /* SD_H_ */