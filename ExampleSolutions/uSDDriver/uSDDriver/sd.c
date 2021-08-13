#include "sd.h"

static DWORD fptr_pos;
static FATFS *pfs;
static char entry_str[BUFFER_SIZE];
FRESULT res;
UINT brTemp, bwTemp;


void sd__mount(FATFS* fs)
{
	res = pf_mount(fs);
	uart__print_disk_error(res, MOUNT);
	pfs = fs;
}

void sd__open(const char* path)
{
	res = pf_open(path); //File name found in conf.h
	uart__print_disk_error(res, OPEN);
	fptr_pos = pfs->fptr;
}

#if SD_USE_READ
void sd__read_br(void* buff, UINT btr, UINT* br)
{
	memset(buff,'\0', btr); //Sets all of buff to null char to fix gibberish @ end of read
	res = pf_read(buff, btr, br);
	uart__print_disk_error(res, READ);
}

void sd__read(void* buff, UINT btr)
{
	sd__read_br(buff, btr, &brTemp);
}
#endif

void sd__write_bw(const void* buff, UINT btw, UINT* bw)
{
	res = pf_write(buff, btw, bw);
	uart__print_disk_error(res, WRITE);
	res = pf_write(0, 0, bw);
	uart__print_disk_error(res, WRITE);
}

void sd__write(const void* buff, UINT btw)
{
	sd__write_bw(buff, btw, &bwTemp);
}

void sd__new_entry(const void* buff, UINT btw, UINT* bw)
{
	memset(entry_str, '\0', BUFFER_SIZE);
	strcpy(entry_str, buff);
	entry_str[BUFFER_SIZE-2] = '\r';
	entry_str[BUFFER_SIZE-1] = '\n';
	pf_lseek(fptr_pos);
	pf_write(entry_str, btw, bw);
	
	/* If EOF, add new line characters to end of entry*/
	if (*bw < BUFFER_SIZE)
	{
		pf_write(0, 0, bw); //Wrap up pfs write operation
	}
	
	fptr_pos += BUFFER_SIZE;
}