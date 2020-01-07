// checksum.cpp : Defines the entry point for the console application.

/*

Compile command:
	mingw32-gcc.exe -o tools\windows\checksum.exe tools\windows\src\checksum.c -static

*/

#if 1

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	FILE *fp;
	unsigned int checksum = 0;
	//unsigned char buf[502000]; //MAX FW Size = 512K-44K/2 = 501760
	unsigned char *buf;
	int i;
	int size = 0;

	if(argc != 2)	return -1;

	fp = fopen(argv[1], "rb+");
	if(!fp)	return -2;


	fseek(fp,0L,SEEK_END);
	size = ftell(fp);
	fseek(fp,0L,SEEK_SET);
	
	buf = malloc(size+100);
	if(!buf){
		fclose(fp);
		return -3;
	}
	
	printf("size = %d \n\r", size);

	memset(buf, 0, size+100);

	fread(buf, 1, size, fp);

	for(i=0;i<size;i++){
		checksum += buf[i];
	}
	
	fseek(fp,0L,SEEK_END);
	
	//fwrite(&checksum, sizeof(int), 1, fp);

	printf("checksum %x\n\r", checksum);

	free(buf);
	fclose(fp);
}

#else

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
	//FILE *fp;
	int fd;	
	unsigned int checksum = 0;
	
	if(argc != 2)	return -1;

	//fp = fopen(argv[1], "r+");
	//if(!fp)	return -2;
	fd = open(argv[1], O_RDWR);
	if(fd<0) return -2;

	//fseek(fp, 0, SEEK_SET);
	lseek(fd, 0, SEEK_SET);
	int size = lseek(fd, 0, SEEK_END);
	lseek(fd, 0, SEEK_SET);

	printf("size = %d \n\r", size);

	unsigned char buf[size+100];

	memset(buf, 0, size+100);

	read(fd, buf, size);

	for(int i=0;i<size;i++){
		checksum += buf[i];
	}
	
	lseek(fd, 0, SEEK_END);
	
	write(fd, &checksum, sizeof(int));

	printf("checksum %x\n\r", checksum);
	close(fd);
}
#endif
