#ifndef TFTP_H
#define TFTP_H

#define port 69
#define ERROR -1

#define RRQ 1
#define WRQ 2
#define DATA 3
#define ACK 0x04
#define ERR 0x05

#define BLOCK_SIZE (512+4)

typedef struct _tftp{
    void   (*recv_handle)(unsigned char *buffer,int len, unsigned int index);
    void   (*send_handle)(unsigned char *buffer,int *len, unsigned int index);
    const char *tftp_file_name;
    const char *tftp_mode;
    const char *tftp_host;
    int tftp_port;
    unsigned char tftp_buf[BLOCK_SIZE];
    int tftp_op;
    unsigned int tftp_retry_num;
    unsigned int tftp_timeout;
}tftp;

int tftp_client_start(tftp *tftp_handler);
int tftp_server_start(tftp *tftp_handler);

#endif