#ifndef _FILE_READER_
#define _FILE_READER_
struct flash_file{
	const char* array_data;
	long current_index;
	long size;
};

typedef struct flash_file flash_file;

void f_open(flash_file* file, const char* array_name, int file_size);
void f_read(flash_file* file, char* buffer, int byte_to_read, int* byte_read);
unsigned char f_eof(flash_file* file);
void f_lseek(flash_file* file, int offset);
void f_close(flash_file* file);

#endif