#ifndef _FF_API_H
#define	_FF_API_H

/* FATFS including files*/
#include "ff.h"

typedef enum path_ops_cmd{
	MOVE_TO_ROOT = 0,
	MOVE_TO,
	MOVE_BACK,
	MOVE_NEXT
}path_ops_cmd_t;

unsigned int cmd_disk_getcapacity(void);

/* FATFS API CONTROL*/
unsigned int cmd_fatfs_list_director(void);
unsigned int cmd_fatfs_init_volume(BYTE* d_name,BYTE status);
unsigned int cmd_fatfs_show_status(void);
unsigned int cmd_fatfs_open_file(char *filename, BYTE mode);
unsigned int cmd_fatfs_close_file(void);
unsigned int cmd_fatfs_chmod(TCHAR *filename, BYTE attri);
unsigned int cmd_fatfs_rename(TCHAR *oldname, TCHAR *newname);
unsigned int cmd_fatfs_seek_pointer(DWORD offset, BYTE mode);
unsigned int cmd_fatfs_read_file(char * read_buf, unsigned int length);
unsigned int cmd_fatfs_write_file(char *write_buf, unsigned int length);
unsigned int cmd_fatfs_truncate_file(void);
unsigned int cmd_fatfs_make_file(TCHAR *filename);
unsigned int cmd_fatfs_make_director(TCHAR *direname);
unsigned int cmd_fatfs_delete_obj(TCHAR *objname);
unsigned int cmd_fatfs_sync(void);
#endif