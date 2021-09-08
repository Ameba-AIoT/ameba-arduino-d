/*

Compile:
windows 10 (mingw64 installed , 32 bits app generated):
mingw32-g++.exe -o upload_image_tool_windows.exe upload_image_tool.cpp -static
i686-w64-mingw32-g++.exe -o upload_image_tool_windows.exe upload_image_tool_windows.cpp -static

linux (ubuntu 20.04 TLS ,64 bits app generated):
g++ -o upload_image_tool_linux upload_image_tool.cpp

macos(Catalina, 64 bits app generated):
g++ -o upload_image_tool_macos upload_image_tool.cpp

*/


#include <iostream>
#include <cstring>
#include <unistd.h>
#include <time.h>

#if defined(__linux__) || defined(__APPLE__)
#include <fcntl.h>
#endif

using namespace std;

#pragma pack(1)

#if defined(__WIN32__) // MINGW64
#include <windows.h>
#define FD HANDLE
#define sleep_ms(s) msleep(s)
#elif defined(__linux__) || defined(__APPLE__)// ubuntu 32 bits  and OS X 64bits
#include <sys/ioctl.h>
#include <termios.h>
#define FD int
#define sleep_ms(s) msleep(s) //usleep((s*1000))
#else
#error compiler is not supported!
#endif

#define SIZEOF_ARRAY(x)  ((int)(sizeof(x)/sizeof(x[0])))
#define delay_ms(s) sleep_ms(s)

#define SPEED_UP 1

//#Flash Download start

//#calibration_ok:[2:29:11]
//LOG_TX Serial port test!!!
//LOG_TX Serial port test!!!

uint8_t auto_flash = true;
// verbose = 0
//           1
//           3
//           5 full debug information
//           7 full debug information with receviced packets
uint8_t verbose = 0;
int baudrate = 0;

typedef struct {
  int speed;
  int sub_cmd;
} SPEED;

SPEED rs232_port[] ={ \
{1500000 ,0x18},
{1444400 ,0x17},
{1382400 ,0x16},
{1000000 ,0x15},
{ 921600 ,0x14},
{ 500000 ,0x13},
{ 460800 ,0x12},
{ 380400 ,0x11},
{ 230400 ,0x10},
{ 153600 ,0x0F},
{ 380400 ,0x0E},
{ 128000 ,0x0D},
{ 115200 ,0x0C}
};

#if defined(__WIN32__)

DCB SerialParams = {0};

#elif defined(__linux__) || defined(__APPLE__)

#if defined(__APPLE__)
#ifndef B1500000
#define B1500000 1500000
#endif
#ifndef B1000000
#define B1000000 1000000
#endif
#ifndef B921600
#define B921600 921600
#endif
#ifndef B500000
#define B500000 500000
#endif
#ifndef B460800
#define B460800 460800
#endif
#endif

SPEED baudrate_defined[] = { \
{1500000 ,B1500000},
#ifdef B1444400
{1444400 ,B1444400},
#endif
#ifdef B1382400
{1382400 ,B1382400},
#endif
{1000000 ,B1000000},
{ 921600 ,B921600},
{ 500000 ,B500000},
{ 460800 ,B460800},
#ifdef B380400
{ 380400 ,B380400},
#endif
{ 230400 ,B230400},
#ifdef B153600
{ 153600 ,B153600},
#endif
#ifdef B380400
{ 380400 ,B380400},
#endif
#ifdef B128000
{ 128000 ,B128000},
#endif
{ 115200 ,B115200}
};
#endif


SPEED max_speed = {0};

#define RET_SUCCESS 0x06
#define RET_SYNC   0x15

uint8_t cmd_buff[16];
uint8_t recv_buff[4096];

#define SPI_FLASH_SIZE (4*1024*1024UL) // 4M bytes Flash
uint8_t *spi_flash_base;
uint8_t *flashloader_base;

//char km0_boot[] = "km0_boot_all.bin";
//char km4_boot[] = "km4_boot_all.bin";
//char km0_km4_app[] = "km0_km4_image2.bin";
//char flashloader_bin[] = "imgtool_flashloader_amebad.bin";

typedef struct {
  int addr;
  long size;
  //char * file;
  uint32_t checksum;
} FLASH_MAP;

FLASH_MAP flashloader = {0,0/*,flashloader_bin*/};

FLASH_MAP flash_map[] = {
{0x8000000,0x0/*,km0_boot*/},
{0x8004000,0x0/*,km4_boot*/},
{0x8006000,0x0/*,km0_km4_app*/}
};

int msleep(long msec) {
    struct timespec ts;
    int res;

    if (msec < 0) {
        errno = EINVAL;
        return -1;
    }

    ts.tv_sec = msec / 1000;
    ts.tv_nsec = (msec % 1000) * 1000000;

    do {
        res = nanosleep(&ts, &ts);
    } while (res && errno == EINTR);

    return res;
}

/*
    Input:
        level : bit0=0 RTS# signal low
                bit1=0 DTR# signal low
        
*/
int set_DTR_RTS(FD fd, unsigned short level)
{
#if defined(__WIN32__)

	if (level & 0x1) 
        EscapeCommFunction(fd, CLRRTS);
	else 
        EscapeCommFunction(fd, SETRTS);

	if (level & 0x2) 
        EscapeCommFunction(fd, CLRDTR);
	else 
        EscapeCommFunction(fd, SETDTR);

#elif defined(__linux__) || defined(__APPLE__)

  int status;
  
  ioctl(fd, TIOCMGET, &status);

  if(level & 0x1)
    status &= ~TIOCM_RTS;
  else
    status |= TIOCM_RTS;

  if(level & 0x2)
    status &= ~TIOCM_DTR;
  else
    status |= TIOCM_DTR;

  ioctl(fd, TIOCMSET, &status);

#endif
    return 0;
}

void reset_method(FD fd , int method) {

    // DTR RTS EN BUN
    //   1  1   1  1
    //   0  0   1  1
    //   1  0   0  1
    //   0  1   1  0

    if(method) {

        //set_DTR_RTS(fd,0x3);
        //sleep_ms(2000);

        // enter uart flash download mode.
        set_DTR_RTS(fd,0x2); // EN LOW
        sleep_ms(500);
        set_DTR_RTS(fd,0x1); // TX_LOG LOW
        sleep_ms(200);
        set_DTR_RTS(fd,0x3); // normal
        sleep_ms(500);

    } else {
        // reset system.
        set_DTR_RTS(fd,0x2); // EN LOW
        sleep_ms(500);
        set_DTR_RTS(fd,0x3); // normal
        //sleep_ms(500);
    }

}

void generate_reset_to_flash( FD fd) {
    reset_method(fd,1);
}
void generate_reset_to_boot( FD fd) {
    reset_method(fd,0);
}

#if defined(__WIN32__)

DWORD read_rs232(FD fd, uint8_t * buffer, int buffersize)
{
    DWORD dwBytesRead = 0;
    buffersize = 1;
    if(!ReadFile(fd, buffer, buffersize, &dwBytesRead, NULL)){
        printf("rs232 read error.\n");
    }
    if(verbose >5 && dwBytesRead)printf("%02X ",*buffer);
    return dwBytesRead;
}


DWORD write_rs232(FD fd, uint8_t * data, int length)
{

	DWORD dwBytesWrite = 0;
	if(!WriteFile(fd, data, length, &dwBytesWrite, NULL)){
		printf("rs232 write error.\n");
	}
	return dwBytesWrite;
}

#elif defined(__linux__) || defined(__APPLE__)

int read_rs232(FD fd, uint8_t * buffer, int len)
{
    if (read(fd,buffer,1) == 1) {
        if(verbose >5)printf("%02X ",*buffer);
        return 1;
    }
    return 0;
}

int write_rs232(FD fd, uint8_t * buffer, int len)
{
    return write(fd, buffer , len);
}
#endif

inline 
bool wait_sync_char(FD fd, int len) {

    int n = len;
    int count;
    int wait=5000;
    int tick=0;

    while(len && wait) {

        if(wait) wait--;
        if(!wait) { printf("** sync timeout.\n");return false;}

        count = read_rs232( fd, recv_buff,1/*sizeof(recv_buff)*/);
        if(count == 0) {
           
        } else if(count==1) {
            if(recv_buff[0] != RET_SYNC) {
                if(len == n && n > 1) {
                    if(!tick)printf(".");
                    tick++;
                    tick%=4;
                    continue;
                } else
                    return false;
            } else {
                len--;
            }
        }
        delay_ms(1);
    }
    return true;
}

inline
bool wait_response_char(FD fd , uint8_t *ret) {

    int n = 5000;
    int count;

    while(n--) {
        if(!n) break;
        count = read_rs232( fd, recv_buff, 1/*sizeof(recv_buff)*/);

        if(count==1) {
            if(recv_buff[0] == RET_SUCCESS) {
                return true;
            }
            *ret = recv_buff[0];
            return false;
        }
        delay_ms(1);
    }
    return false;
}

bool change_rs232_speed(FD fd, int s) {

#if defined(__WIN32__)
    //DWORD temp;   
    //COMSTAT ComState;

    // claer error status    
    //ClearCommError(fd, &temp, &ComState); 

    SerialParams = {0};

    SerialParams.DCBlength=sizeof(SerialParams);
    if (!GetCommState(fd, &SerialParams)) {
        //could not get the state of the comport
        printf("eror...\n");
    }

    //SetupComm(fd, 256, 256);
    SetupComm(fd, 4096, 4096);

    BOOL  Status = SetCommMask(fd, EV_TXEMPTY);

    SerialParams.BaudRate=s;
    SerialParams.ByteSize=8;
    SerialParams.StopBits=ONESTOPBIT;
    SerialParams.Parity=NOPARITY;

    SerialParams.fDtrControl = DTR_CONTROL_DISABLE;
    SerialParams.fRtsControl = RTS_CONTROL_DISABLE;
    
    if(!SetCommState(fd, &SerialParams)) {
        return false;
        //printf("error to change serial port speed.\n");
    }

    COMMTIMEOUTS timeouts={0};

if(0){//(s != 115200) {
    timeouts.ReadIntervalTimeout=0;
    timeouts.ReadTotalTimeoutConstant=1;
    timeouts.ReadTotalTimeoutMultiplier=0;
    timeouts.WriteTotalTimeoutConstant=0;
    timeouts.WriteTotalTimeoutMultiplier=0;
} else {
    timeouts.ReadIntervalTimeout=-1;
    timeouts.ReadTotalTimeoutConstant=2000;
    timeouts.ReadTotalTimeoutMultiplier=-1;
    timeouts.WriteTotalTimeoutConstant=0;
    timeouts.WriteTotalTimeoutMultiplier=0;
}
    //timeouts.ReadIntervalTimeout=-1;
    //timeouts.ReadTotalTimeoutConstant=1000;
    //timeouts.ReadTotalTimeoutMultiplier=-1;
    //timeouts.WriteTotalTimeoutConstant=0;
    //timeouts.WriteTotalTimeoutMultiplier=0;

    //timeouts.ReadIntervalTimeout=0;
    //timeouts.ReadTotalTimeoutConstant=1000;
    //timeouts.ReadTotalTimeoutMultiplier=0;
    //timeouts.WriteTotalTimeoutConstant=1000;
    //timeouts.WriteTotalTimeoutMultiplier=0;

    if(!SetCommTimeouts(fd, &timeouts)){
        //handle error
        printf("timeout init error.\n");
        return false;
    }

    DCB r = {0};
    GetCommState(fd, &r);
    if(r.BaudRate != s) return false;


    //PurgeComm (fd, PURGE_RXCLEAR);

#elif defined(__linux__) || defined(__APPLE__)

    int i;
    for(i=0; i< sizeof(baudrate_defined)/sizeof(baudrate_defined[0]) ;i++)
    {
        if(s == baudrate_defined[i].speed) {
            break;
        }
    }

    if(i >= SIZEOF_ARRAY(baudrate_defined)) {
        //printf("baudrate doesn't supported.\n");
        return false;
    }

    struct  termios options;
    struct  termios r;

    tcgetattr(fd, &options);

    bzero(&options, sizeof(options));

    cfsetispeed(&options,baudrate_defined[i].sub_cmd);
    cfsetospeed(&options,baudrate_defined[i].sub_cmd);

    // set raw input
    options.c_cflag     |= (CLOCAL | CREAD | CS8);
    //options.c_cflag &= ~CRTSCTS;
    options.c_iflag |=  ( IGNPAR | IGNBRK );
    //options.c_lflag     &= ~(ICANON | ECHO | ECHOE | ISIG);
    //options.c_oflag     &= ~OPOST;
    options.c_cc[VMIN]  = 0;
    //options.c_cc[VTIME] = 1; // 0.1sec
    options.c_cc[VTIME] = 100; // 1s

    // set the options
    if(tcsetattr(fd,TCSANOW,&options) != 0) {
        //printf("serial port init failed.\n");
        return false;
    }

    // check success or not.
    tcgetattr(fd, &r);
    speed_t rr = cfgetispeed(&r);
    if( baudrate_defined[i].sub_cmd != rr) return false;
    
    //tcflush(fd, TCIFLUSH);
#endif

    return true;
}

bool flush_rs232(FD fd) {

    int count=1;
    uint8_t flush_buf[4096];
    int len = 300; // force recevied 300 chars to timeout.
    while(len && count) {
        count = read_rs232(fd, flush_buf, 1/*sizeof(flush_buf)*/);
        if(count == 0 ) break;
        if(count == 1 && flush_buf[0] == RET_SYNC) break;
        len--;
    }

    if(!len) return false;
#if defined(__WIN32__)
    PurgeComm(fd, PURGE_TXCLEAR | PURGE_RXCLEAR);
#elif defined(__linux__) || defined(__APPLE__)
    tcflush(fd,TCIOFLUSH);
#endif
    return true;
}

//inline
bool send_cmd(FD fd,uint8_t *buf,int size) {

    int count = 1;
    uint8_t ret;

    if(verbose >5) printf("send command %02X len=%d\n",*buf,size);

    //flush_rs232(fd);

    if(*buf != 0x07 ) if(!wait_sync_char(fd, 1)) return false;
    //if(*buf == 0x04 ) wait_sync_char(fd, 1);

    count = write_rs232(fd, buf , size);
/*
    while(size) {
        count = write_rs232(fd, buf , 1);
        buf++;
        size--;
        //delay_ms(1);
        }
*/
xxx:
    if( !wait_response_char(fd,&ret)) {
        //if(ret == 0x86) goto xxx;
        if(ret == RET_SYNC) {
            goto xxx;
        }

        printf("[send_cmd] *** error for response char 0x%02X.\n",ret);
        return false;
    }
    return true;
}

bool verify_cmd(FD fd,uint8_t *buf,int size , uint32_t *ret) {

    int count;
    int i = 0;
    int wait = 5000;
    uint8_t c;
    *ret = 0;

    //flush_rs232(fd);

    if(!wait_sync_char(fd,1)) { printf("****[verify_cmd]****.\n"); return false;}

    count = write_rs232(fd, buf , size);

/*    
    while(size) {
        count = write_rs232(fd, buf , 1);
        buf++;
        size--;
        //delay_ms(1);
    }
*/
    while(wait--) {
        if(!wait) {printf("verify timeout.\n"); return false; }
    
        count = read_rs232(fd, &c, 1 );

        if(count) {
            // first char is sync char. just continue
            if(i==0 && c == RET_SYNC) continue;
            // first char should be 0x27 for certain response.
            if(i==0 && c != 0x27) return false;

            if(i) *(((uint8_t *)ret)+i-1) = c;
            i++;
        }

        if(i==5) break;
        delay_ms(1);
    }
    if(i != 5) return false;
    return true;
}

uint8_t uint8_checksum(uint8_t * buf, int len) {

    uint8_t ret = 0xFF;
    for(int i= 0; i< len; i++) ret += *(buf+i);
    return ret;
}

int load_file(FLASH_MAP *f, char *name , int addr , uint8_t * ptr) {

    FILE *fp;
    int count;

    fp=fopen(name,"rb");
    if(fp == NULL) {
        printf("open file %s error.\n", name);
            return false;
    }

    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    f->size = size;
    f->addr = addr;

    count = fread(ptr,1,size,fp);

    uint32_t checksum = 0;

    int tail = size/4;
    if(size%4 == 0) {
    } else if(size%4 == 1) {
        checksum = (*((uint32_t *)ptr+tail) & 0xFF);
    } else if(size%4 == 2){
        checksum = (*((uint32_t *)ptr+tail) & 0xFFFF);
    } else {
        checksum = (*((uint32_t *)ptr+tail) & 0xFFFFFF);
    }

    for(int i = 0; i < (size/4); i++) {
        checksum += *((uint32_t *)ptr+i);
    }

    f->checksum = checksum;

    fclose(fp);
    if(verbose > 3) printf("loading file %s, will write address 0x%X to 0x%lX, checksum = %08X.\n", 
                                    name,
                                    (f->addr & ~0x8000000),
                                    (f->addr & ~0x8000000) + f->size,
                                    f->checksum );

    return true;
}

bool write_1k(FD fd, uint8_t sequence_id, int mcu_addr, uint8_t * ptr) {

    uint8_t write_buff[1032+100];
    uint8_t ret;

    write_buff[0] = 0x2; // command
    write_buff[1] = sequence_id;
    write_buff[2] = ~sequence_id;
    *(uint32_t *)&write_buff[3] = mcu_addr;

    memcpy(write_buff+7 , ptr , 1024);

    write_buff[1031] = uint8_checksum(write_buff , 1031);

    //printf("checksum = 0x%02X\n",write_buff[1031]);
    write_rs232(fd,write_buff,1032);

    if( !wait_response_char(fd,&ret)) {
        printf("[write_1k] *** error for response char 0x06.\n");
        return false;
    }

    return true;
}

bool write_block(int bar,FD fd, uint8_t *id, int mcu_addr, long size,uint8_t * package) {

    int tick=1;
    int i;

    if(verbose >1) printf("write addr 0x%X size 0x%lX.\n",mcu_addr,size);

#if SPEED_UP
    uint8_t *ptr = (uint8_t *)malloc(1032*((size+1023)/1024));
    uint8_t ret;
    bool flag = true;

    for(int i=0; i< (size+1023)/1024 ; i++) {
        *(ptr+i*1032+0) = 0x2; // command
        *(ptr+i*1032+1) = *id;
        *(ptr+i*1032+2) = ~(*id);
        *(uint32_t *)(ptr+i*1032+3) = mcu_addr +i*1024;

        memcpy(ptr+i*1032+7 , (package+i*1024) , 1024);

        *(ptr+i*1032+1031) = uint8_checksum(ptr+i*1032 , 1031);
        (*id)++;
    }

    if(!wait_sync_char(fd,1)) { printf("[write_block] error for char 0x15\n"); return false;}
    for(int i=0; i< (size+1023)/1024 ; i++) {
        write_rs232(fd,ptr+i*1032,1032);

        if( !wait_response_char(fd,&ret)) {
            printf("[write_1k] *** error for response char 0x06.\n");
            //return false;
            flag = false;
            break;
        }
        if(bar && tick==0) printf("#");
        //if(bar) printf(".");
        tick++;
        tick%=8;
    }
    if(bar) printf("\n");

    free(ptr);
    return flag;

#else

    if(!wait_sync_char(fd,1)) return false;

    for(int i=0; i< (size+1023)/1024 ; i++) {
        if(!write_1k (fd, *id, mcu_addr + i*1024, package + i*1024) ) {
            printf("program block failed.\n");
            return false;
        }
        if(bar && tick==0) printf(".");
        tick++;
        tick%=8;
        (*id)++;
    }
    if(bar) printf("\n");
    return true;
#endif
}

bool erase_block(FD fd, int addr,long size) {

    printf("erase addr: 0x%07X, size:%ldKB.\n", addr,size/1024);

    cmd_buff[0] = 0x17;
    *(uint32_t *)&cmd_buff[1] = addr;
    *(uint32_t *)&cmd_buff[4] = (size+4095)/4096;

    //wait_sync_char(fd,1);
    if(!send_cmd(fd,cmd_buff,6)) return false;
    return true;
}

inline
bool set_max_speed(FD fd,int s) {

    uint8_t sub_cmd = 0;

    for(int i=0; i< SIZEOF_ARRAY(rs232_port); i++) {
        if(rs232_port[i].speed == s) {
            sub_cmd = rs232_port[i].sub_cmd;
            break;
        }
    }
    if(sub_cmd == 0) {
        printf("%d not supported. change to 115200.\n",s);
        s= 115200; // for default.
    }

    if(s != 115200) {

        if(!wait_sync_char(fd,1)) return false;
    
        cmd_buff[0] = 0x05;
        cmd_buff[1] = sub_cmd;
        send_cmd(fd,cmd_buff,2);
        //if( !wait_response_char(fd,&ret)) printf("error for respons char 0x06.\n");
        change_rs232_speed(fd,s);
        //delay_ms(500);
        //flush_rs232(fd);
     
    }

    cmd_buff[0] = 0x07;
    if(!send_cmd(fd,cmd_buff,1)) return false;
    
    if(!wait_sync_char(fd,1)) return false;

    return true;
}

bool program_spi_flash(FD fd ,int s) {

#if defined(__WIN32__)
    DWORD temp;   
    COMSTAT ComState;

    // claer error status    
    ClearCommError(fd, &temp, &ComState); 
#endif
    if(!flush_rs232(fd)) { printf("flush timeout happened!.\n"); return false; }
    printf("**");

    //00 0D 23 46 6C 61 73 68 20 44 6F 77 6E 6C 6F 61 64 20 53 74 61 72 74 20 0A 0D 15
    // <TODO> parse string "#Flash Download start"

    if(verbose>5) printf("** before first sync responsed **\n");

    // wait 2 sync characters for confirm rtlxxx is in flash mode.
    if(!wait_sync_char(fd,2)) { printf("\n[00]sync char 0x15 error!.\n"); return false;}
    printf("\n");

    if(verbose>5) printf("** after first sync responsed **\n");

    // change rs232 to maximum speed.
    printf("set baudrate to %d.\n",s);
    if(!set_max_speed(fd,s)) return false;

    //if( !wait_response_char(fd,&ret)) printf("error for respons char 0x06.\n");
    //cmd_buff[0] = 0x07;
    //if(send_cmd(fd,cmd_buff,1)) return false;

    printf("upload flash download bootloader to ram.\n");
    uint8_t id=1;

    // load flash bootloader into ram.
    if(!write_block(0, fd, &id, flashloader.addr,flashloader.size, flashloader_base)) {
        printf("flash bootloader was failed.\n");
        return false;
    }

    //wait_sync_char(fd,1);

    cmd_buff[0] = 0x04;
    if(!send_cmd(fd,cmd_buff,1)) return false;


    printf("re-init and set baudrate to 115200.\n");
    // re-init
    change_rs232_speed(fd,115200);
    //flush_rs232(fd);

    if(!wait_sync_char(fd,2)) { printf("\n****[01]****.\n"); return false;}
    printf("\n");

    // erase flash.
    cmd_buff[0] = 0x26;
    cmd_buff[1] = 0x01;
    cmd_buff[2] = 0x01;
    cmd_buff[3] = 0x00;
    if(!send_cmd(fd,cmd_buff,4)) return false;
    
    if(!erase_block(fd, flash_map[0].addr,flash_map[0].size)) { printf("erase block failed.\n");return false;}
    if(!erase_block(fd, flash_map[1].addr,flash_map[1].size)) { printf("erase block failed.\n");return false;}
    if(!erase_block(fd, flash_map[2].addr,flash_map[2].size)) { printf("erase block failed.\n");return false;}

    if(!wait_sync_char(fd,1)) { printf("****[02]****.\n"); return false; }
    // program application & km0,km4 bootloader

    // change rs232 to maximum speed.
    printf("set baudrate to %d.\n",s);
    if(!set_max_speed(fd,s)) return false;

    //cmd_buff[0] = 0x07;
    //send_cmd(fd,cmd_buff,1);
    //wait_sync_char(fd,1);

    id=1;

    // load km0 bootloader
    if(!write_block(0, fd, &id, flash_map[0].addr,flash_map[0].size, spi_flash_base + (flash_map[0].addr & ~0x8000000) )) {
        printf(".\n");
        return false;
    } else printf("km0 bootloader has been sent successfully.\n");

    if(!write_block(0, fd, &id, flash_map[1].addr,flash_map[1].size, spi_flash_base + (flash_map[1].addr & ~0x8000000) )) {
        printf("km0 boot wrote failed.\n");
        return false;
    } else printf("km4 bootloader has been sent successfully.\n");

    if(!write_block(1, fd, &id, flash_map[2].addr,flash_map[2].size, spi_flash_base + (flash_map[2].addr & ~0x8000000) )) {
        printf("wrote app was failed.\n");
        return false;
    } else printf("app has been sent successfully.\n");

    //wait_sync_char(fd,1);

    cmd_buff[0] = 0x04;
    if(!send_cmd(fd,cmd_buff,1)) return false;

    // re-init again.
    change_rs232_speed(fd,115200);
    flush_rs232(fd);

    if(!wait_sync_char(fd,1)) { printf("****[03]****.\n"); return false; }

    printf("verifying km0 km4 and app blocks....");
    // verify flash checksum.
    for(int i = 0 ; i< SIZEOF_ARRAY(flash_map); i++) {

        cmd_buff[0] = 0x27;
        *(uint32_t *)&cmd_buff[1] = flash_map[i].addr & ~0x8000000;
        *(uint32_t *)&cmd_buff[4] = flash_map[i].size;

        uint32_t checksum;

        if(!verify_cmd(fd,cmd_buff,7,&checksum)) return false;

        if(checksum != flash_map[i].checksum) {
            printf("checksum failed.\n");
            return false;
        }
    }
    printf("ok.\n");

    return true;
}

void flash_image(char* com) {

    char buf[256];
    char c;
    char last_c;
    FD fd;
    
#if defined(__WIN32__)
    
    strcpy(buf,"\\\\.\\");
    strcat(buf,com);
    
    fd = CreateFileA(buf,                 //port name
            GENERIC_READ | GENERIC_WRITE, //Read/Write
            0,                            // No Sharing
            NULL,                         // No Security
            OPEN_EXISTING,                // Open existing port only
            0,                            // Non Overlapped I/O
            NULL);                        // Null for Comm Devices
    
    
    if (fd == INVALID_HANDLE_VALUE) {
        printf("Error in opening serial port.\n");
        return;
    }

    // check available speed support.
    for(int i=0 ; i < SIZEOF_ARRAY(rs232_port); i++) {
    
        if(verbose >5) printf("check speed %d ... ",rs232_port[i].speed);
        if(change_rs232_speed(fd,rs232_port[i].speed)) {
            
            if(verbose >5) printf("supported.\n");
    
            // workarournd for my CP2102.
            if(rs232_port[i].speed == 1000000) continue;
    
            if(max_speed.speed == 0) {

                max_speed.speed = rs232_port[i].speed;
                max_speed.sub_cmd = rs232_port[i].sub_cmd;
                break;
            }       
    
        } else { if(verbose >5)printf("\n");}
    
    }


    change_rs232_speed(fd, 115200);


#elif defined(__linux__) || defined(__APPLE__)
    
        fd = open( com , O_RDWR | O_NOCTTY | O_NDELAY);
        if (fd <0) {
           printf("Error in opening serial port %s.\n",com);
           return;
        }

        // Open the device in nonblocking mode
        fcntl(fd, F_SETFL, FNDELAY);

        // check available speed support.
        for(int i=0 ; i < SIZEOF_ARRAY(baudrate_defined); i++) {

            if(verbose >5) printf("check speed %d ... ",baudrate_defined[i].speed);
            if(change_rs232_speed(fd,baudrate_defined[i].speed)) {
                
                if(verbose >5) printf("supported.\n");

                // workarournd for my CP2102.
                if(baudrate_defined[i].speed == 1000000) continue;

                if(max_speed.speed == 0)
                for(int ii=0 ; ii < SIZEOF_ARRAY(rs232_port);ii++) {
                    if(rs232_port[ii].speed == baudrate_defined[i].speed) {
                        max_speed.speed = rs232_port[ii].speed;
                        max_speed.sub_cmd = rs232_port[ii].sub_cmd;
                        break;
                    }
                }       

            } else { if(verbose >5)printf("\n");}

        }
    
#endif

    //max_speed.speed = 1500000;
    //max_speed.sub_cmd = 0x18;
    if(verbose >5) printf("max_speed = %d cmd = 0x%02X\n",max_speed.speed,max_speed.sub_cmd);

    spi_flash_base = (uint8_t *) malloc(SPI_FLASH_SIZE); // 4M flash.
    memset(spi_flash_base,0xFF,SPI_FLASH_SIZE);

    flashloader_base = (uint8_t *) malloc(1024*8); // 8K ram.
    memset(flashloader_base,0xFF,1024*8);

    load_file(&flashloader,(char *)"imgtool_flashloader_amebad.bin",  0x082000, flashloader_base);

    load_file(&flash_map[0],(char *)"km0_boot_all.bin",  0x8000000, spi_flash_base);
    load_file(&flash_map[1],(char *)"km4_boot_all.bin",  0x8004000, spi_flash_base + 0x4000);
    load_file(&flash_map[2],(char *)"km0_km4_image2.bin",0x8006000, spi_flash_base + 0x6000);

    printf("set baudrate to 115200.\n");
    change_rs232_speed(fd,115200);

    if(auto_flash) {
        printf("enter download flash mode.\n");
        generate_reset_to_flash(fd);
    }

    bool status;
    if(baudrate) {
        if(baudrate < 115200) {
            baudrate = 115200;
            printf("high speed mode doesn't support. swith to 115200 as default.\n");
        }
        status = program_spi_flash(fd,baudrate);
    }
    else
        status = program_spi_flash(fd,max_speed.speed);

    if(auto_flash) {
      printf("hard resetting via RTS pin..\n");
      generate_reset_to_boot(fd);
    }
    free(spi_flash_base);
    free(flashloader_base);
#if defined(__WIN32__)
    CloseHandle(fd); //Closing the Serial Port
#elif defined(__linux__) || defined(__APPLE__)
    close(fd);
#endif
    if(status) printf("done.\n");
    else printf("failed.");
}



int main(int argc, char *argv[]) {
    string cmd;
    int ret;

    int index = 3;
    
    while(argc >= index+1) {

        if(strncmp(argv[index],"--baudrate=",strlen("--baudrate="))==0) {

            baudrate = atoi(argv[index]+strlen("--baudrate="));
            printf("*** force baudrate to %d ***\n",baudrate);

        } else if(strncmp(argv[index],"--verbose=",strlen("--verbose="))==0) {

            verbose = atoi(argv[index]+strlen("--verbose="));
            printf("*** set verbose=%d ***\n",verbose);

        } else if(strncmp(argv[index],"--auto=",strlen("--auto="))==0) {

            auto_flash = atoi(argv[index]+strlen("--auto="));
            printf("*** auto = %d ***\n",auto_flash);
        }
        index++;
    }
   
    // change directory to {runtime.tools.ameba_d_tools.path}
    ret = chdir(argv[1]);

    if(!auto_flash) {
      // 5 seconds count down to allow user setting ameba D to UART download mode
      cmd = "Please enter the upload mode (wait 5s)";
      cout << cmd << endl;
      for (int i = 5; i > 0; i--) {
        //sleep(1);
        sleep_ms(1000);
        cmd = to_string(i);
        cout << "    0" << cmd << endl;
      }
    }


    setbuf(stdout, NULL);
    flash_image(argv[2]);

    return 0;
}


