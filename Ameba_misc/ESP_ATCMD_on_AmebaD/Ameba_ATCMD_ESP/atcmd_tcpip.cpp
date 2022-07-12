#include "atcmd_core.h"
#include "atcmd_tcpip.h"
#include "atcmd_wifi.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef uint32_t in_addr_t;

#include "lwip_netconf.h"
#include "lwip/opt.h"
#include "lwip/sockets.h"
#include "lwip/inet.h"
#include "lwip/inet_chksum.h"
#include "lwip/netdb.h"
#include "lwip/prot/icmp.h"
#include "lwip/prot/ip.h"
#include "lwip/prot/ip4.h"

extern struct netif xnetif[NET_IF_NUM];

#ifdef __cplusplus
}
#endif

extern atcmd_cwstate_t cwstate;

uint8_t cip_mux = 0;
uint8_t cip_mode = 0;
uint8_t cip_dinfo = 0;
uint16_t cip_reconnintv = 1;
uint8_t passthrough_mode = 0;
uint16_t transmission_mode = 0;
uint32_t atcmd_tcpip_passthrough_last_tick = 0;
TaskHandle_t atcmd_tcpip_receive_handle = NULL;
TaskHandle_t atcmd_tcpip_passthrough_send_handle = NULL;
SemaphoreHandle_t atcmd_tcpip_tx_sema = NULL;

atcmd_client_conn_t client_conn_list[ATCMD_MAX_CLIENT_CONN];
uint8_t atcmd_tcpip_tx_buf[ATCMD_TCPIP_TX_BUFFER_SIZE];     // RX from UART, send to network
uint8_t atcmd_tcpip_rx_buf[ATCMD_TCPIP_RX_BUFFER_SIZE];     // RX from network, sent to UART
volatile uint16_t atcmd_tcpip_tx_buflen = 0;

void atcmd_init_conn_struct(atcmd_client_conn_t* conn) {
    memset(conn, 0, sizeof(atcmd_client_conn_t));
    conn->sockfd = INVALID_SOCKET_ID;
}

void atcmd_tcpip_init(void) {
    memset(client_conn_list, 0, sizeof(client_conn_list));

    // initialize semaphore used to indicate data received for transmission
    vSemaphoreCreateBinary(atcmd_tcpip_tx_sema);
    xSemaphoreTake(atcmd_tcpip_tx_sema, 1/portTICK_PERIOD_MS);

    atcmd_tcpip_register();
}

int atcmd_tcpip_send_data(uint8_t link_id, int send_size) {
    int ret = 0, error_no = 0;

    // Send data according to connection type
    if (client_conn_list[link_id].protocol == CONN_MODE_TCP) {              // TCP case
        if (send_size < (ATCMD_TCPIP_TX_BUFFER_SIZE / 2)) {
            ret = lwip_send(client_conn_list[link_id].sockfd, atcmd_tcpip_tx_buf, send_size, 0);
        } else {
            // Split into two lwip_send calls, since it errors when send_size > approx 6500
            if ( (ret = lwip_send(client_conn_list[link_id].sockfd, atcmd_tcpip_tx_buf, (ATCMD_TCPIP_TX_BUFFER_SIZE / 2), MSG_MORE)) ) {
                ret += lwip_send(client_conn_list[link_id].sockfd, &(atcmd_tcpip_tx_buf[ATCMD_TCPIP_TX_BUFFER_SIZE / 2]),
                    send_size - (ATCMD_TCPIP_TX_BUFFER_SIZE / 2), 0);
            }
        }
        if (ret <= 0) {
            error_no = lwip_getsocklasterr(client_conn_list[link_id].sockfd);
            if (AT_DEBUG) printf("[atcmd_tcpip_send_data] ERROR:Failed to send data ret = %d | error = %d\r\n", ret, error_no);
        }
    } else if (client_conn_list[link_id].protocol == CONN_MODE_SSL) {       // SSL case

    } else {                                                                // UDP case

    }
    return ret;
}

void atcmd_tcpip_passthrough_send_task(void* param) {
    (void)param;
    uint32_t current_time;
    while (passthrough_mode) {
        if (xSemaphoreTake(atcmd_tcpip_tx_sema, 5/portTICK_PERIOD_MS) == pdTRUE) {
            // Buffer almost full, immediate transmission
            if (client_conn_list[0].active != CONN_ROLE_INACTIVE) {
                atcmd_uart_irq_disable();
                atcmd_tcpip_send_data(0, atcmd_tcpip_tx_buflen);
                atcmd_tcpip_tx_buflen = 0;
                atcmd_uart_irq_enable();
            } else {
                // Discard data received while waiting for passthrough reconnection
                atcmd_tcpip_tx_buflen = 0;
            }
        } else {
            if (atcmd_tcpip_tx_buflen == 0) {
                continue;
            }
            // Check if 20ms has elapsed since last receive data byte
            current_time = xTaskGetTickCount();
            if ((current_time - atcmd_tcpip_passthrough_last_tick) < 20) {
                continue;
            }
            atcmd_uart_irq_disable();
            // Check for passthrough exit sequence
            if ((atcmd_tcpip_tx_buflen == 3) && (memcmp(atcmd_tcpip_tx_buf, "+++", 3) == 0)) {
                passthrough_mode = 0;
                atcmd_tcpip_tx_buflen = 0;
                atcmd_uart_irq_enable();
                break;
            }

            // Transmit data
            if (client_conn_list[0].active != CONN_ROLE_INACTIVE) {
                atcmd_tcpip_send_data(0, atcmd_tcpip_tx_buflen);
                atcmd_tcpip_tx_buflen = 0;
            } else {
                // Discard data received while waiting for passthrough reconnection
                atcmd_tcpip_tx_buflen = 0;
            }
            atcmd_uart_irq_enable();
        }
    }
    // Not in passthrough mode, stop atcmd_tcpip_passthrough_send_task
    atcmd_tcpip_passthrough_send_handle = NULL;
    vTaskDelete(NULL);
}

uint8_t atcmd_client_start(uint8_t link_id);
int atcmd_tcpip_receive_data(uint8_t link_id, int* recv_size) {
    struct timeval tv;
    fd_set readfds;
    int error_no = 0, ret = 0, size = 0;

    // Check to see if socket has data to read
    FD_ZERO(&readfds);
    FD_SET(client_conn_list[link_id].sockfd, &readfds);
    tv.tv_sec = 0;
    tv.tv_usec = 20000;     // read timeout of 20ms
    ret = lwip_select(client_conn_list[link_id].sockfd + 1, &readfds, NULL, NULL, &tv);
    if (!( (ret > 0) && (FD_ISSET(client_conn_list[link_id].sockfd, &readfds)) )) {
        // No receive event
        *recv_size = size;
        return error_no;
    }

    // Receive data according to connection type
    if (client_conn_list[link_id].protocol == CONN_MODE_TCP) {              // TCP case
        size = lwip_recv(client_conn_list[link_id].sockfd, atcmd_tcpip_rx_buf, ATCMD_TCPIP_RX_BUFFER_SIZE, 0);

        if (size == 0) {
            // ERROR:Connection closed
            error_no = 7;
        } else if (size < 0) {
            // "ERROR:Failed to receive data.ret = -0x%x!", -size
            error_no = 8;
        }
    } else if (client_conn_list[link_id].protocol == CONN_MODE_SSL) {       // SSL case

    } else {                                                                // UDP case

    }

    // Check for receive errors and close connection
    if (error_no == 0) {
        *recv_size = size;
    } else {
        if (passthrough_mode) {
            // Auto reconnect when in passthrough mode
            client_conn_list[link_id].active = CONN_ROLE_INACTIVE;
            uint16_t reconn_wait = 0;
            while ((passthrough_mode) && (reconn_wait < cip_reconnintv)) {
                reconn_wait++;
                vTaskDelay(100/portTICK_PERIOD_MS);
            }
            // If still in passthrough mode after waiting for reconnection interval, reconnect
            if (passthrough_mode) {
                atcmd_client_start(link_id);
                return error_no;
            }
        }

        if (cip_mux) {
            at_printf("%d,", link_id);
        }
        at_printf("CLOSED\r\n");
        if (client_conn_list[link_id].protocol == CONN_MODE_SSL) {
            // Close SSL connection
        } else {
            lwip_close(client_conn_list[link_id].sockfd);
        }
        atcmd_init_conn_struct(&(client_conn_list[link_id]));
    }
    return error_no;
}

void atcmd_tcpip_receive_task(void* param) {
    (void)param;
    uint8_t active_conn = 0;

    while (1) {
        active_conn = 0;
        for (uint8_t link_id = 0; link_id < ATCMD_MAX_CLIENT_CONN; link_id++) {
            if (client_conn_list[link_id].active != CONN_ROLE_INACTIVE) {
                active_conn++;
                int error_no = 0;
                int recv_size = 0;
                error_no = atcmd_tcpip_receive_data(link_id, &recv_size);
                if ((error_no == 0) && recv_size) {
                    atcmd_tcpip_rx_buf[recv_size] = '\0';
                    if (!cip_mode) {
                        at_printf("\r\n+IPD");
                        if (cip_mux) {
                            at_printf(",%d", link_id);
                        }
                        at_printf(",%d", recv_size);
                        if (cip_dinfo) {
                            at_printf("\",%s\"", inet_ntoa(client_conn_list[link_id].addr));
                            at_printf(",%d", client_conn_list[link_id].port);
                        }
                        at_printf(":");
                    }
                    at_print_data(atcmd_tcpip_rx_buf, recv_size);
                    if (!cip_mode) {
                        at_printf("\r\n");
                    }
                    vTaskDelay(20/portTICK_PERIOD_MS);
                }
            }
        }

        if (active_conn == 0) {
            break;
        }
        taskYIELD();
//        vTaskDelay(20/portTICK_PERIOD_MS);
    }

    // No active connections, stop atcmd_tcpip_receive_task
    atcmd_tcpip_receive_handle = NULL;
    vTaskDelete(NULL);
}

uint8_t atcmd_client_start(uint8_t link_id) {
    int sockfd;

    if (client_conn_list[link_id].protocol == CONN_MODE_TCP) {              // TCP case
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd == INVALID_SOCKET_ID) {
            if (AT_DEBUG) printf("[atcmd_client_start] Failed to create socket\r\n");
            return ATCMD_ERROR;
        }

        // initialize dest structure
        struct sockaddr_in serv_addr;
        memset(&serv_addr, 0, sizeof(serv_addr));
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_addr.s_addr = client_conn_list[link_id].addr;
        serv_addr.sin_port = htons(client_conn_list[link_id].port);

        client_conn_list[link_id].sockfd = sockfd;

        if (connect(sockfd, ((struct sockaddr*)&serv_addr), sizeof(serv_addr)) == 0) {
            int enable = 1;
            int disable = 0;
            int timeout = 3000;     // 3 secs

            int ca_len = sizeof(serv_addr);
            if (lwip_getsockname(sockfd, ((struct sockaddr*)&serv_addr), (socklen_t *)&ca_len) == 0) {
                client_conn_list[link_id].local_port = ntohs(serv_addr.sin_port);
            } else {
                lwip_close(sockfd);
                atcmd_init_conn_struct(&(client_conn_list[link_id]));
                return ATCMD_ERROR;
            }
            if (client_conn_list[link_id].keep_alive) {
                lwip_setsockopt(sockfd, SOL_SOCKET, SO_KEEPALIVE, &enable, sizeof(enable));
                lwip_setsockopt(sockfd, IPPROTO_TCP, TCP_KEEPIDLE, &(client_conn_list[link_id].keep_alive), sizeof(client_conn_list[link_id].keep_alive));
            } else {
                lwip_setsockopt(sockfd, SOL_SOCKET, SO_KEEPALIVE, &disable, sizeof(disable));
            }
            lwip_setsockopt(sockfd, IPPROTO_TCP, TCP_NODELAY, &enable, sizeof(enable));
            lwip_setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
            lwip_setsockopt(sockfd, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout));

            client_conn_list[link_id].active = CONN_ROLE_CLIENT;

            // start task to process incoming data
            if (atcmd_tcpip_receive_handle == NULL) {
                int result;
                result = xTaskCreate(atcmd_tcpip_receive_task, "atcmd_lwip_rx_task", 1024, NULL, tskIDLE_PRIORITY + 3, &atcmd_tcpip_receive_handle);
                if (result != pdPASS) {
                    if (AT_DEBUG) printf("[atcmd_client_start] atcmd_lwip_rx_task create failed\r\n");
                    lwip_close(sockfd);
                    atcmd_init_conn_struct(&(client_conn_list[link_id]));
                    return ATCMD_ERROR;
                }
            }
            if (!passthrough_mode) {
                if (cip_mux) {
                    at_printf("%d,", link_id);
                }
                at_printf("CONNECT\r\n");
            }
        } else {
            lwip_close(sockfd);
            atcmd_init_conn_struct(&(client_conn_list[link_id]));
            return ATCMD_ERROR;
        }
        
    } else if (client_conn_list[link_id].protocol == CONN_MODE_SSL) {       // SSL case

    } else {                                                                // UDP case

    }

    return ATCMD_OK;
}

//---------------------------- Commands for TCP/IP ATCMD functionality ----------------------------//

uint8_t e_AT_CIPSTATUS(void *arg) {
    // Obtain the TCP/UDP/SSL Connection Status and Information
    // AT+CIPSTATUS
    // STATUS:<stat>
    // +CIPSTATUS:<link ID>,<"type">,<"remote IP">,<remote port>,<local port>,<tetype>
    (void)arg;
    // <stat>: status of the ESP station interface.
    //0: WiFi off or STA mode not initialized.
    //1: WiFI STA mode initialized, but not started a Wi-Fi connection.
    //2: WiFI STA mode is connected to an AP and obtained an IP address.
    //3: Has created a TCP/SSL transmission.
    //4: All of the TCP/UDP/SSL connections of the device are disconnected.
    //5: WiFI STA mode started a Wi-Fi connection, but was not connected to an AP or disconnected from an AP.

    uint8_t status;
    switch (cwstate) {
        case CWSTATE_INITIAL: {
            status = 1;
            break;
        }
        case CWSTATE_CONNECTEDIP: {
            status = 2;
            for (uint8_t i = 0; i < ATCMD_MAX_CLIENT_CONN; i++) {
                if (client_conn_list[i].sockfd != 0) {
                    // initial state of sockets = 0, any non zero socket indicate previous use
                    status = 4;
                }
                if (client_conn_list[i].active != CONN_ROLE_INACTIVE) {
                    status = 3;
                    break;
                }
            }
            break;
        }
        case CWSTATE_CONNECTED:
        case CWSTATE_CONNECTING:
        case CWSTATE_DISCONNECTED: {
            status = 5;
            break;
        }
    }
    at_printf("STATUS:%d\r\n", status);

    const char* conn_type = NULL;
    char* addr = NULL;
    uint8_t tetype;
    for (uint8_t i = 0; i < ATCMD_MAX_CLIENT_CONN; i++) {
        if (client_conn_list[i].active != CONN_ROLE_INACTIVE) {
            switch (client_conn_list[i].protocol) {
                case CONN_MODE_TCP:
                    conn_type = "TCP";
                    break;
                case CONN_MODE_UDP:
                    conn_type = "UDP";
                    break;
                case CONN_MODE_SSL:
                    conn_type = "SSL";
                    break;
            }
            addr = inet_ntoa(client_conn_list[i].addr);
            if (client_conn_list[i].active == CONN_ROLE_CLIENT) {
                tetype = 0;
            } else {
                tetype = 1;
            }
            at_printf("+CIPSTATUS:%d,\"%s\",\"%s\",%d,%d,%d\r\n", i, conn_type, addr, client_conn_list[i].port, client_conn_list[i].local_port, tetype);
        }
    }
    return ATCMD_OK;
}

uint8_t s_AT_CIPDOMAIN(void *arg) {
    // Resolve a Domain Name
    // AT+CIPDOMAIN=<"domain name">[,<ip network>]
    // +CIPDOMAIN:<"IP address">
    (void)arg;
    uint8_t argc = 0;
    char* argv[ATCMD_MAX_ARG_CNT] = {0};
    char* domain = NULL;      // Arg 1
    uint8_t ip_network = 0;
    struct in_addr addr;
    struct hostent *server_host;

    if (!arg) {
        return ATCMD_ERROR;
    }
    if ((argc = atcmd_parse_params((char*)arg, argv)) > 3) {
        return ATCMD_ERROR;
    }

    domain = argv[1];
    if (argv[2] != NULL) {
        ip_network = atoi(argv[2]);
        if ((ip_network < 1) || (ip_network > 3)) {
            return ATCMD_ERROR;
        }
    }
    if (ip_network == 3) {
        // IPV6 resolution currently not supported
        return ATCMD_ERROR;
    }

    if (inet_aton(domain, &addr)) {
        // valid IP address provided, return same address
        at_printf("+CIPDOMAIN:\"%s\"\r\n", inet_ntoa(addr));
    } else {
        // Not a valid IP address provided, attempt to DNS resolve hostname
        server_host = gethostbyname(domain);
        if (server_host){
            memcpy(&addr, server_host->h_addr, 4);
            at_printf("+CIPDOMAIN:\"%s\"\r\n", inet_ntoa(addr));
        } else {
            return ATCMD_ERROR;
        }
    }
    return ATCMD_OK;
}

uint8_t s_AT_CIPSTART(void *arg) {
    // Establish TCP Connection, UDP Transmission, or SSL Connection
    // TCP:
      // Single connection (AT+CIPMUX=0):
        // AT+CIPSTART=<"type">,<"remote host">,<remote port>[,<keep alive>][,<"local IP">]
      // Multiple Connections (AT+CIPMUX=1):
        // AT+CIPSTART=<link ID>,<"type">,<"remote host">,<remote port>[,<keep alive>][,<"local IP">]
    // UDP:
      // Single connection (AT+CIPMUX=0):
        // AT+CIPSTART=<"type">,<"remote host">,<remote port>[,<local port>,<mode>,<"local IP">]
      // Multiple connections (AT+CIPMUX=1):
        // AT+CIPSTART=<link ID>,<"type">,<"remote host">,<remote port>[,<local port>,<mode>,<"local IP">]
    // SSL:
      // Single connection (AT+CIPMUX=0):
        // AT+CIPSTART=<"type">,<"remote host">,<remote port>[,<keep alive>,<"local IP">]
      // Multiple connections (AT+CIPMUX=1):
        // AT+CIPSTART=<link ID>,<"type">,<"remote host">,<remote port>[,<keep alive>,<"local IP">]
    (void)arg;
    uint8_t argc = 0;
    char* argv[ATCMD_MAX_ARG_CNT] = {0};

    uint8_t link_id = 0;
    char* conn_type = NULL;
    char* remote_host = NULL;
    int remote_port = 0;
    uint16_t keep_alive = 0;
    char* local_ip = NULL;
    int local_port = 0;
    uint8_t udp_mode = 0;

    if (!arg) {
        return ATCMD_ERROR;
    }
    argc = atcmd_parse_params((char*)arg, argv);
    if (cip_mux) {
        if ((argc < 5) || (argc > 8)) {
            return ATCMD_ERROR;
        }
    } else {
        if ((argc < 4) || (argc > 7)) {
            return ATCMD_ERROR;
        }
    }

    if (AT_DEBUG) {
        for (uint8_t i = 1; i < argc; i++) {
            if (argv[i] != NULL) {
                at_printf("Arg %d: %s\r\n", i, argv[i]);
            }
        }
    }

    // Cannot start new connection if passthrough mode is enable and a connection already exist
    if ((cip_mode) && (atcmd_tcpip_receive_handle != NULL)) {
        return ATCMD_ERROR;
    }

    // Link ID
    if (cip_mux) {
        if (argv[1] != NULL) {
            link_id = atoi(argv[1]);
            if (link_id >= ATCMD_MAX_CLIENT_CONN) {
                return ATCMD_ERROR;
            }
        } else {
            return ATCMD_ERROR;
        }
    }
    // Check if Link ID is already connected
    if (client_conn_list[link_id].active != CONN_ROLE_INACTIVE) {
        return ATCMD_ERROR;
    }

    // Connection Type
    // cip_mux = 0 or 1, used as offset
    atcmd_conn_mode_t conn_mode = CONN_MODE_TCP;
    if (argv[1 + cip_mux] != NULL) {
        conn_type = argv[1 + cip_mux];
        if (strlen(conn_type) == 0) {
            return ATCMD_ERROR;
        }
        if (strcmp(conn_type, "TCP") == 0) {
            conn_mode = CONN_MODE_TCP;
        } else if (strcmp(conn_type, "UDP") == 0) {
//            conn_mode = CONN_MODE_UDP;
            return ATCMD_ERROR;
        } else if (strcmp(conn_type, "SSL") == 0) {
//            conn_mode = CONN_MODE_SSL;
            return ATCMD_ERROR;
        } else {
            return ATCMD_ERROR;
        }
        client_conn_list[link_id].protocol = conn_mode;
    } else {
        return ATCMD_ERROR;
    }

    // Remote Host
    struct in_addr remote_addr;
    struct hostent* server_host;
    if (argv[2 + cip_mux] != NULL) {
        remote_host = argv[2 + cip_mux];
        if (inet_aton(remote_host, &remote_addr) == 0) {
            // Not a valid IP address provided, attempt to DNS resolve hostname
            server_host = gethostbyname(remote_host);
            if (server_host){
                memcpy(&remote_addr, server_host->h_addr, 4);
            } else {
                return ATCMD_ERROR;
            }
        }
        memcpy(&(client_conn_list[link_id].addr), &remote_addr, 4);
    } else {
        return ATCMD_ERROR;
    }

    // Remote Port
    if (argv[3 + cip_mux] != NULL) {
        remote_port = atoi(argv[3 + cip_mux]);
        if (remote_port < 0 || remote_port > 65535) {
            return ATCMD_ERROR;
        }
        client_conn_list[link_id].port = remote_port;
    } else {
        return ATCMD_ERROR;
    }

    if ((conn_mode == CONN_MODE_TCP) || (conn_mode == CONN_MODE_SSL)) {
        // TCP Keep Alive
        if (argv[4 + cip_mux] != NULL) {
            keep_alive = atoi(argv[4 + cip_mux]);
            if (keep_alive > 7200) {
                return ATCMD_ERROR;
            }
            client_conn_list[link_id].keep_alive = keep_alive;
        }
        // Local IP
        if (argv[5 + cip_mux] != NULL) {
            struct in_addr local_addr;
            local_ip = argv[5 + cip_mux];
            if (inet_aton(local_ip, &local_addr) == 0) {
                return ATCMD_ERROR;
            }
            memcpy(&(client_conn_list[link_id].local_addr), &local_addr, 4);
        }
    } else {
        // Local Port
        if (argv[4 + cip_mux] != NULL) {
            local_port = atoi(argv[4 + cip_mux]);
            if (local_port < 0 || local_port > 65535) {
                return ATCMD_ERROR;
            }
            client_conn_list[link_id].local_port = local_port;
        }
        // UDP Mode
        if (argv[5 + cip_mux] != NULL) {
            udp_mode = atoi(argv[5 + cip_mux]);
            if (udp_mode > 2) {
                return ATCMD_ERROR;
            }
        }
        // Local IP
        if (argv[6 + cip_mux] != NULL) {
            struct in_addr local_addr;
            local_ip = argv[6 + cip_mux];
            if (inet_aton(local_ip, &local_addr) == 0) {
                return ATCMD_ERROR;
            }
            memcpy(&(client_conn_list[link_id].local_addr), &local_addr, 4);
        }
    }

    return atcmd_client_start(link_id);
}

uint8_t s_AT_CIPSEND(void *arg) {
    // Set the data length to be send in the Normal Transmission Mode.
    // Single connection: (AT+CIPMUX=0)
    // AT+CIPSEND=<length>
    // Multiple connections: (AT+CIPMUX=1)
    // AT+CIPSEND=<link ID>,<length>
    // Remote host and port can be set for UDP transmission:
    // AT+CIPSEND=[<link ID>,]<length>[,<"remote host">,<remote port>]
    (void)arg;
    uint8_t argc = 0;
    char* argv[ATCMD_MAX_ARG_CNT] = {0};
    uint8_t link_id = 0;
    uint16_t data_len = 0;
    char* remote_host = NULL;
    int remote_port = 0;

    if (cip_mode == 1) {
        return ATCMD_ERROR;
    }
    if (!arg) {
        return ATCMD_ERROR;
    }
    argc = atcmd_parse_params((char*)arg, argv);
    if (cip_mux) {
        if ((argc < 2) || (argc > 5)) {
            return ATCMD_ERROR;
        }
    } else {
        if ((argc < 2) || (argc > 4)) {
            return ATCMD_ERROR;
        }
    }

    // Link ID
    if (cip_mux) {
        if (argv[1] != NULL) {
            link_id = atoi(argv[1]);
            if (link_id >= ATCMD_MAX_CLIENT_CONN) {
                return ATCMD_ERROR;
            }
        } else {
            return ATCMD_ERROR;
        }
    }
    // Check if Link ID is already connected
    if (client_conn_list[link_id].active == CONN_ROLE_INACTIVE) {
        return ATCMD_ERROR;
    }

    // Data length
    if (argv[1 + cip_mux] != NULL) {
        data_len = atoi(argv[1 + cip_mux]);
        if ((data_len > ATCMD_TCPIP_TX_BUFFER_SIZE) || (data_len == 0)) {
            return ATCMD_ERROR;
        }
    } else {
        return ATCMD_ERROR;
    }

    // Remote Host
    struct in_addr remote_addr;
    struct hostent* server_host;
    if (argv[2 + cip_mux] != NULL) {
        remote_host = argv[2 + cip_mux];
        if (inet_aton(remote_host, &remote_addr) == 0) {
            // Not a valid IP address provided, attempt to DNS resolve hostname
            server_host = gethostbyname(remote_host);
            if (server_host){
                memcpy(&remote_addr, server_host->h_addr, 4);
            } else {
                return ATCMD_ERROR;
            }
        }
    }

    // Remote Port
    if (argv[3 + cip_mux] != NULL) {
        remote_port = atoi(argv[3 + cip_mux]);
        if (remote_port < 0 || remote_port > 65535) {
            return ATCMD_ERROR;
        }
    }

    int ret = 0;
    // Enter normal transmission mode
    atcmd_tcpip_tx_buflen = 0;
    transmission_mode = data_len;
    at_printf("\r\nOK\r\n\r\n>");
    // wait for semaphore indicating data for transmission is received
    while (xSemaphoreTake(atcmd_tcpip_tx_sema, portMAX_DELAY) != pdTRUE);
    at_printf("\r\nRecv %d bytes\r\n", atcmd_tcpip_tx_buflen);
    ret = atcmd_tcpip_send_data(link_id, atcmd_tcpip_tx_buflen);
    atcmd_tcpip_tx_buflen = 0;
    if (ret != data_len) {
        return ATCMD_ERROR;
    }
    at_printf("\r\nSEND OK\r\n");
    return ATCMD_NORSP;
}

uint8_t e_AT_CIPSEND(void *arg) {
    // Enter the Passthrough Transmission Mode.
    // AT+CIPSEND
    (void)arg;
    if ((cip_mode == 0) || (cip_mux == 1)) {
        return ATCMD_ERROR;
    }
    // Check if Link ID is already connected
    if (client_conn_list[0].active == CONN_ROLE_INACTIVE) {
        return ATCMD_ERROR;
    }

    // Enter passthrough transmission mode
    int timeout = 20;     // 20ms
    lwip_setsockopt(client_conn_list[0].sockfd, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout));
    atcmd_tcpip_tx_buflen = 0;
    atcmd_tcpip_passthrough_last_tick = 0;
    passthrough_mode = 1;
    int result;
    result = xTaskCreate(atcmd_tcpip_passthrough_send_task, "atcmd_lwip_pt_tx_task", 1024, NULL, tskIDLE_PRIORITY + 3, &atcmd_tcpip_passthrough_send_handle);
    if (result != pdPASS) {
        if (AT_DEBUG) printf("[e_AT_CIPSEND] atcmd_tcpip_passthrough_send_task create failed\r\n");
        passthrough_mode = 0;
        atcmd_tcpip_tx_buflen = 0;
        return ATCMD_ERROR;
    }
    at_printf("\r\nOK\r\n\r\n>");
    return ATCMD_NORSP;
}

uint8_t s_AT_CIPCLOSE(void *arg) {
    // Close TCP/UDP/SSL connection in the multiple connections mode.
    (void)arg;
    uint8_t argc = 0;
    char* argv[ATCMD_MAX_ARG_CNT] = {0};
    uint8_t link_id = 0;

    if (cip_mux == 0) {
        return ATCMD_ERROR;
    }
    if (!arg) {
        return ATCMD_ERROR;
    }
    argc = atcmd_parse_params((char*)arg, argv);
    if (argc != 2) {
        return ATCMD_ERROR;
    }

    // Link ID
    if (argv[1] != NULL) {
        link_id = atoi(argv[1]);
        if (link_id >= ATCMD_MAX_CLIENT_CONN) {
            return ATCMD_ERROR;
        }
    } else {
        return ATCMD_ERROR;
    }

    if (client_conn_list[link_id].active == CONN_ROLE_INACTIVE) {
        return ATCMD_ERROR;
    }
    // Set as inactive first to prevent duplicate close message from lwip_receive_task
    client_conn_list[link_id].active = CONN_ROLE_INACTIVE;
    vTaskDelay(20/portTICK_PERIOD_MS);
    if (lwip_close(client_conn_list[link_id].sockfd) != 0) {
        return ATCMD_ERROR;
    }
    atcmd_init_conn_struct(&(client_conn_list[link_id]));
    at_printf("%d,CLOSED\r\n", link_id);
    return ATCMD_OK;
}

uint8_t e_AT_CIPCLOSE(void *arg) {
    // Close TCP/UDP/SSL connection in the single connection mode.
    (void)arg;

    if (cip_mux == 1) {
        return ATCMD_ERROR;
    }
    if (client_conn_list[0].active == CONN_ROLE_INACTIVE) {
        return ATCMD_ERROR;
    }
    // Set as inactive first to prevent duplicate close message from lwip_receive_task
    client_conn_list[0].active = CONN_ROLE_INACTIVE;
    vTaskDelay(20/portTICK_PERIOD_MS);
    if (lwip_close(client_conn_list[0].sockfd) != 0) {
        return ATCMD_ERROR;
    }
    atcmd_init_conn_struct(&(client_conn_list[0]));
    at_printf("CLOSED\r\n");
    return ATCMD_OK;
}

uint8_t e_AT_CIFSR(void *arg) {
    // Obtain the Local IP Address and MAC Address
    (void)arg;
    //<”APIP”>: IPv4 address of Wi-Fi softAP interface
    //<”APIP6LL”>: Linklocal IPv6 address of Wi-Fi softAP interface
    //<”APIP6GL”>: Global IPv6 address of Wi-Fi softAP interface
    //<”APMAC”>: MAC address of Wi-Fi softAP interface

    //<”STAIP”>: IPv4 address of Wi-Fi station interface
    //<”STAIP6LL”>: Linklocal IPv6 address of Wi-Fi station interface
    //<”STAIP6GL”>: Global IPv6 address of Wi-Fi station interface
    //<”STAMAC”>: MAC address of Wi-Fi station interface
    uint8_t* mac = LwIP_GetMAC(&xnetif[0]);
    uint8_t* ip = LwIP_GetIP(&xnetif[0]);
    if (cwstate != CWSTATE_CONNECTEDIP) {
        at_printf("+CIFSR:STAIP,\"%d.%d.%d.%d\"\r\n", 0, 0, 0, 0);
    } else {
        at_printf("+CIFSR:STAIP,\"%d.%d.%d.%d\"\r\n", ip[0], ip[1], ip[2], ip[3]);
    }
    at_printf("+CIFSR:STAMAC,\"%02x:%02x:%02x:%02x:%02x:%02x\"\r\n", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    return ATCMD_OK;
}

uint8_t q_AT_CIPMUX(void *arg) {
    // Query current multiple connection setting
    // AT+CIPMUX?
    (void)arg;
    at_printf("+CIPMUX:%d\r\n", cip_mux);
    return ATCMD_OK;
}

uint8_t s_AT_CIPMUX(void *arg) {
    // Configure multiple connection setting
    // AT+CIPMUX=<mode>
    (void)arg;
    uint8_t argc = 0;
    char* argv[ATCMD_MAX_ARG_CNT] = {0};
    uint8_t mux = 0;          // Arg 1

    if (!arg) {
        return ATCMD_ERROR;
    }
    argc = atcmd_parse_params((char*)arg, argv);
    if (argc != 2) {
        return ATCMD_ERROR;
    }

    mux = atoi(argv[1]);
    if (mux > 1) {
        return ATCMD_ERROR;
    }

    if ((cip_mode == 1) && (mux == 1)) {
        return ATCMD_ERROR;
    }
    // Check if server is currently running before configuration is allowed

    // Check for any open connections first before configuration is allowed
    for (uint8_t link_id = 0; link_id < ATCMD_MAX_CLIENT_CONN; link_id++) {
        if (client_conn_list[link_id].active != CONN_ROLE_INACTIVE) {
            return ATCMD_ERROR;
        }
    }
    cip_mux = mux;
    return ATCMD_OK;
}

uint8_t q_AT_CIPMODE(void *arg) {
    // Query the transmission mode.
    // AT+CIPMODE?
    (void)arg;
    at_printf("+CIPMODE:%d\r\n", cip_mode);
    return ATCMD_OK;
}

uint8_t s_AT_CIPMODE(void *arg) {
    // Set the transmission mode.
    // AT+CIPMODE=<mode>
    (void)arg;
    uint8_t argc = 0;
    char* argv[ATCMD_MAX_ARG_CNT] = {0};
    uint8_t mode = 0;          // Arg 1

    if (!arg) {
        return ATCMD_ERROR;
    }
    argc = atcmd_parse_params((char*)arg, argv);
    if (argc != 2) {
        return ATCMD_ERROR;
    }

    mode = atoi(argv[1]);
    if (mode > 1) {
        return ATCMD_ERROR;
    }

    // Check for active connection count before configuration is allowed
    uint8_t active_conns = 0;
    for (uint8_t link_id = 0; link_id < ATCMD_MAX_CLIENT_CONN; link_id++) {
        if (client_conn_list[link_id].active != CONN_ROLE_INACTIVE) {
            active_conns++;
        }
    }
    if (active_conns > 1) {
        return ATCMD_ERROR;
    }

    cip_mode = mode;
    return ATCMD_OK;
}

uint8_t q_AT_CIPDINFO(void *arg) {
    // Query “+IPD” Message Mode.
    // AT+CIPDINFO?
    (void)arg;
    at_printf("+CIPDINFO:");
    if (cip_dinfo) {
        at_printf("true");
    } else {
        at_printf("false");
    }
    return ATCMD_OK;
}

uint8_t s_AT_CIPDINFO(void *arg) {
    // Set “+IPD” Message Mode.
    // AT+CIPDINFO=<mode>
    (void)arg;
    uint8_t argc = 0;
    char* argv[ATCMD_MAX_ARG_CNT] = {0};
    uint8_t info = 0;          // Arg 1

    if (!arg) {
        return ATCMD_ERROR;
    }
    argc = atcmd_parse_params((char*)arg, argv);
    if (argc != 2) {
        return ATCMD_ERROR;
    }

    info = atoi(argv[1]);
    if (info > 1) {
        return ATCMD_ERROR;
    }

    cip_dinfo = info;
    return ATCMD_OK;
}

uint8_t q_AT_CIPRECONNINTV(void *arg) {
    // Query the automatic connect interval for the Wi-Fi Passthrough Mode.
    // AT+CIPRECONNINTV?
    (void)arg;
    at_printf("+CIPRECONNINTV:%d\r\n", cip_reconnintv);
    return ATCMD_OK;
}

uint8_t s_AT_CIPRECONNINTV(void *arg) {
    // Set the automatic reconnecting interval when TCP/UDP/SSL transmission breaks in the Wi-Fi Passthrough Mode.
    // AT+CIPRECONNINTV=<interval>
    (void)arg;
    uint8_t argc = 0;
    char* argv[ATCMD_MAX_ARG_CNT] = {0};
    uint16_t reconnintv = 0;          // Arg 1

    if (!arg) {
        return ATCMD_ERROR;
    }
    argc = atcmd_parse_params((char*)arg, argv);
    if (argc != 2) {
        return ATCMD_ERROR;
    }

    reconnintv = atoi(argv[1]);
    if ((reconnintv > 36000) || (reconnintv == 0)) {
        return ATCMD_ERROR;
    }

    cip_reconnintv = reconnintv;
    return ATCMD_OK;
}

uint8_t s_AT_PING(void *arg) {
    // Ping the remote host.
    // AT+PING=<"host">
    (void)arg;
    uint8_t argc = 0;
    char* argv[ATCMD_MAX_ARG_CNT] = {0};

    if (!arg) {
        return ATCMD_ERROR;
    }
    argc = atcmd_parse_params((char*)arg, argv);
    if (argc != 2) {
        return ATCMD_ERROR;
    }

    // Remote Host
    char* remote_host = NULL;
    struct in_addr remote_addr;
    struct hostent* server_host;
    if (argv[1] != NULL) {
        remote_host = argv[1];
        if (inet_aton(remote_host, &remote_addr) == 0) {
            // Not a valid IP address provided, attempt to DNS resolve hostname
            server_host = gethostbyname(remote_host);
            if (server_host){
                memcpy(&remote_addr, server_host->h_addr, 4);
            } else {
                return ATCMD_ERROR;
            }
        }
    } else {
        return ATCMD_ERROR;
    }

    struct sockaddr_in to_addr, from_addr;
    int from_addr_len = sizeof(struct sockaddr);
    int ping_socket = 0;
    int ping_timeout = 1000;
    uint16_t ping_id = 0xABCD;
    static uint8_t seq_id = 0;
    unsigned char ping_buf[(sizeof(struct icmp_echo_hdr) + 64)] = {0};
    unsigned char reply_buf[(sizeof(struct icmp_echo_hdr) + 64)] = {0};
    int ping_size = (sizeof(struct icmp_echo_hdr) + 64);
    int reply_size;
    uint32_t ping_time, reply_time;

    ping_socket = socket(AF_INET, SOCK_RAW, IP_PROTO_ICMP);
    setsockopt(ping_socket, SOL_SOCKET, SO_RCVTIMEO, &ping_timeout, sizeof(ping_timeout));

    to_addr.sin_len = sizeof(to_addr);
    to_addr.sin_family = AF_INET;
    memcpy(&to_addr.sin_addr, &remote_addr, 4);

    // Generate Ping packet
    for(int i = 0; i < 64; i ++) {
        ping_buf[sizeof(struct icmp_echo_hdr) + i] = 0x41 + i;
    }
    struct icmp_echo_hdr* pecho = (struct icmp_echo_hdr*) ping_buf;
    ICMPH_TYPE_SET(pecho, ICMP_ECHO);
    ICMPH_CODE_SET(pecho, 0);
    pecho->id = ping_id;
    pecho->seqno = htons(++seq_id);
    //Checksum includes icmp header and data. Calculate after fill up icmp header
    pecho->chksum = inet_chksum(pecho, ping_size);

    // Send and receive Ping packet
    lwip_sendto(ping_socket, ping_buf, ping_size, 0, (struct sockaddr *) &to_addr, sizeof(to_addr));
    ping_time = xTaskGetTickCount();
    reply_size = recvfrom(ping_socket, reply_buf, ping_size, 0, (struct sockaddr *) &from_addr, (socklen_t*)&from_addr_len);
    if ( (reply_size >= (int)(sizeof(struct ip_hdr) + sizeof(struct icmp_echo_hdr))) && (from_addr.sin_addr.s_addr == to_addr.sin_addr.s_addr) ) {
        reply_time = xTaskGetTickCount();
        struct ip_hdr* iphdr = (struct ip_hdr *)reply_buf;
        pecho = (struct icmp_echo_hdr *)(reply_buf + (IPH_HL(iphdr) * 4));

        if ((pecho->id == ping_id) && (pecho->seqno == htons(seq_id))) {
            at_printf("+PING:%d\r\n", (reply_time - ping_time) * portTICK_RATE_MS);
            close(ping_socket);
            return ATCMD_OK;
        }
    }

    at_printf("+PING:TIMEOUT\r\n");
    close(ping_socket);
    return ATCMD_ERROR;
}

atcmd_command_t atcmd_tcpip_commands[] = {
      {"AT+CIPSTATUS",      NULL,   NULL,               NULL,               e_AT_CIPSTATUS,   {NULL, NULL}},
      {"AT+CIPDOMAIN",      NULL,   NULL,               s_AT_CIPDOMAIN,     NULL,             {NULL, NULL}},
      {"AT+CIPSTART",       NULL,   NULL,               s_AT_CIPSTART,      NULL,             {NULL, NULL}},
      {"AT+CIPSEND",        NULL,   NULL,               s_AT_CIPSEND,       e_AT_CIPSEND,     {NULL, NULL}},
      {"AT+CIPCLOSE",       NULL,   NULL,               s_AT_CIPCLOSE,      e_AT_CIPCLOSE,    {NULL, NULL}},
      {"AT+CIFSR",          NULL,   NULL,               NULL,               e_AT_CIFSR,       {NULL, NULL}},
      {"AT+CIPMUX",         NULL,   q_AT_CIPMUX,        s_AT_CIPMUX,        NULL,             {NULL, NULL}},
      {"AT+CIPMODE",        NULL,   q_AT_CIPMODE,       s_AT_CIPMODE,       NULL,             {NULL, NULL}},
      {"AT+CIPDINFO",       NULL,   q_AT_CIPDINFO,      s_AT_CIPDINFO,      NULL,             {NULL, NULL}},
      {"AT+CIPRECONNINTV",  NULL,   q_AT_CIPRECONNINTV, s_AT_CIPRECONNINTV, NULL,             {NULL, NULL}},
      {"AT+CIPRECVMODE",    NULL,   NULL,    NULL,      NULL,               {NULL, NULL}},
      {"AT+CIPRECVDATA",    NULL,   NULL,    NULL,      NULL,               {NULL, NULL}},
      {"AT+CIPRECVLEN",     NULL,   NULL,    NULL,      NULL,               {NULL, NULL}},
      {"AT+PING",           NULL,   NULL,               s_AT_PING,          NULL,             {NULL, NULL}},
};

void atcmd_tcpip_register(void) {
    atcmd_add_cmdtable(atcmd_tcpip_commands, sizeof(atcmd_tcpip_commands)/sizeof(atcmd_tcpip_commands[0]));
}
