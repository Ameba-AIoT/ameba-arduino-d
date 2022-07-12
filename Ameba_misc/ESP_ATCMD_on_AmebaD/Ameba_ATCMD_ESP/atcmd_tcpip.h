#ifndef ATCMD_TCPIP_H
#define ATCMD_TCPIP_H

#ifdef __cplusplus
extern "C" {
#endif

#include "platform_stdlib.h"

#ifdef __cplusplus
}
#endif

#define ATCMD_MAX_CLIENT_CONN   5
#define INVALID_SOCKET_ID       (-1)

#define ATCMD_TCPIP_TX_BUFFER_SIZE    8192
#define ATCMD_TCPIP_RX_BUFFER_SIZE    2920

typedef enum {
    CONN_MODE_TCP = 0,
    CONN_MODE_UDP = 1,
    CONN_MODE_SSL = 2, 
} atcmd_conn_mode_t;

typedef enum {
    CONN_ROLE_INACTIVE = 0,
    CONN_ROLE_CLIENT   = 1,
    CONN_ROLE_SERVER   = 2,
} atcmd_conn_role_t;

typedef struct atcmd_client_conn
{
    atcmd_conn_role_t active;
    int sockfd;
    atcmd_conn_mode_t protocol;
    uint32_t addr;
    uint16_t port;
    uint32_t local_addr;
    uint16_t local_port;
    uint32_t keep_alive;
} atcmd_client_conn_t;

struct timeval {
  long    tv_sec;         /* seconds */
  long    tv_usec;        /* and microseconds */
};

void atcmd_tcpip_init(void);
void atcmd_tcpip_register(void);

#endif

