#ifndef __TEST_SOCKET_H__
#define __TEST_SOCKET_H__

#include "ms_hal_socket.h"

extern int iFdUDP;
extern int iFdTCP;

typedef struct socket_opt {
    char *msg;
    uint16_t value;
}socket_opt_t;

extern socket_opt_t opt_level[];

extern socket_opt_t opt_name[];

int16_t GetLevel(const char *l);

int16_t GetOptName(const char *l);

int TestSocketCreate(const char *domain, const char *type);

int TestSocketBind(int16_t sock_fd);

int TestSocketConnect(int16_t sock_fd, const char *ipaddr);

void TestSocketSend(const char *ipaddr, const char *send_data);

void TestSocketRecv(void *pvParameters );

void TestSocketRecvFrom(void *pvParameters );

void TestSocketSendTo(const char *ipaddr, const char *send_data);

#endif