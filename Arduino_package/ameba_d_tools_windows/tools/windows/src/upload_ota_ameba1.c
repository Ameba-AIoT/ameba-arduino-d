/*
 * Compiler: MinGW (32bit)
 * 
 * mingw32-gcc.exe -o upload_ota.exe tools\windows\src\upload_ota.c-lwsock32 -static
 *
 **/


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <io.h>
#include <sys/stat.h>
#include "getopt.h"
#include <winsock2.h>

#pragma comment(lib,"ws2_32.lib") //Winsock Library

#define BUF_SIZE 256

#define CMD_FILENAME    0x01
#define CMD_IP          0x02
#define CMD_PORT        0x04
#define CMD_HELP        0x08

#define CHUNK_SIZE 1460

static struct option long_options[] = {
    {"file",      required_argument, 0, 'f'},
    {"interface", required_argument, 0, 'i'},
    {"port",      required_argument, 0, 'p'},
    {"help",      no_argument,       0, 'h'},
    {0,           0,                 0,  0 }
};

void dumpMenu() {
    printf("Usage: upload_ota\r\n");
    printf("  -f  --file       the OTA image to be uploaded\r\n");
    printf("  -i  --interface  the network ip address\r\n");
    printf("  -p  --port       the network port\r\n");
    printf("  -h  --help\r\n");
}

void ota_process(char *filename, char *ipstring, int port) {

    FILE *pFile;
    struct stat info;
    uint32_t ota_description[3];
    long filesize;
    unsigned char *ota_buf = NULL;

    SOCKET s;
    WSADATA wsa;

    uint32_t checksum = 0;
    struct sockaddr_in server;
    int i, n;
    printf("file: %s\r\n", filename);
    printf("ip: %s\r\n", ipstring);
    printf("port: %d\r\n", port);

    do {
        pFile = fopen(filename, "rb");
        if ( pFile == NULL ) {
            printf("ERR: Fail to open file: %s\r\n", filename);
            break;
        }

        // get file size;
        fseek(pFile, 0L, SEEK_END);
        filesize = ftell(pFile);
        fseek(pFile, 0, SEEK_SET);

        ota_buf = (unsigned char *) malloc (filesize);
        if (ota_buf == NULL) {
            printf("ERR: no enough memory\r\n");
            break;
        }
        fread(ota_buf, 1, filesize, pFile);
        fclose(pFile);

        checksum = 0;
        for (i=0; i<filesize; i++) {
            checksum += (ota_buf[i] & 0xff);
        }

        if (WSAStartup(MAKEWORD(2,2),&wsa) != 0) {
            printf("ERR: FAIL to init Winsock, error code: %d\r\n", WSAGetLastError());
            break;
        }

        s = socket(AF_INET, SOCK_STREAM, 0);
        if (s == INVALID_SOCKET) {
            printf("ERR: Could not create socket, error code: %d\r\n",  WSAGetLastError());
            break;
        }

        server.sin_addr.s_addr = inet_addr(ipstring);
        server.sin_family = AF_INET;
        server.sin_port = htons( port );

        printf("Try to connect...\r\n");
        if ( connect(s, (struct sockaddr *)&server, sizeof(server)) < 0 ) {
            printf("ERR: Connect error\r\n");
            break;
        }

        ota_description[0] = checksum;
        ota_description[1] = 0;
        ota_description[2] = (uint32_t) filesize;

        printf("Send OTA info...\r\n");
        n = send(s, (unsigned char *)ota_description, sizeof(ota_description), 0);
        if (n < 0) {
            printf("ERR: fail to send OTA description\r\n");
            break;
        }

        printf("String uploading...\r\n");
        n = send(s, (const char *)ota_buf, filesize, 0);
        if (n < 0) {
            printf("ERR: fail to send OTA image\r\n");
            break;
        }

        closesocket(s);

        WSACleanup();

        printf("Upload success\r\n");

    } while (0);

    if (ota_buf != NULL) {
        free(ota_buf);
        ota_buf = NULL;
    }
}

int main(int argc, char *argv[]) {

    int c;
    int opt_idx = 0;

    unsigned char cmd = 0;

    char filename[BUF_SIZE];
    char ipstring[16];
    char portstring[10];
    int port;

    while (1) {
        c = getopt_long(argc, argv, "f:i:p:h?", long_options, &opt_idx);

        if (c== -1) break;

        switch (c) {
            case 'f':
                sprintf(filename, "%s", optarg);
                cmd |= CMD_FILENAME;
                break;
            case 'i':
                if (strlen(optarg) < 16) {
                    sprintf(ipstring, "%s", optarg);
                    cmd |= CMD_IP;
                }
                break;
            case 'p':
                if (strlen(optarg) < 10) {
                    sprintf(portstring, "%s", optarg);
                    cmd |= CMD_PORT;
                }
                break;
            case 'h':
            case '?':
            default:
                cmd |= CMD_HELP;
                break;
        }
    }

    if (cmd & CMD_HELP) {
        dumpMenu();
    } else if (cmd & (CMD_FILENAME | CMD_IP | CMD_PORT)) {
        port = atoi(portstring);
        ota_process(filename, ipstring, port);
    } else {
        dumpMenu();
    }

    return 0;
}

