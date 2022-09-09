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

#define ATCMD_TCPIP_RX_BUFFER_SIZE    2920     // RX from network, sent to UART
#define ATCMD_TCPIP_TX_BUFFER_SIZE    8192     // RX from UART, send to network

#define DEFAULT_DNS_1   "1.1.1.1"
#define DEFAULT_DNS_2   "8.8.8.8"

#define MBEDTLS_DEBUG_LEVEL 0
#define MBEDTLS_EXPORT_KEY 0

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
    uint32_t addr;              // Target address and port to send to
    uint16_t port;
    uint32_t local_addr;        // Local address and port of connection
    uint16_t local_port;
    uint32_t remote_addr;       // Remote address and port of received packet
    uint16_t remote_port;
    uint32_t keep_alive;
    uint16_t recv_data_len;     // used to keep track of received data pending read in passive receive mode
    uint16_t udp_mode;
    mbedtls_ssl_context* ssl;
    char ssl_cli_sni[64];       // SSL Server name indication
} atcmd_client_conn_t;

struct timeval {
  long    tv_sec;         /* seconds */
  long    tv_usec;        /* and microseconds */
};

typedef uint32_t in_addr_t;

void atcmd_tcpip_init(void);
void atcmd_tcpip_register(void);

#endif

