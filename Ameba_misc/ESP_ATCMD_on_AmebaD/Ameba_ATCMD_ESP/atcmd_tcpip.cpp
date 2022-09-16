#include "atcmd_core.h"
#include "atcmd_tcpip.h"
#include "atcmd_wifi.h"
#include "atcmd_tls_certs.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "wifi_constants.h"

#include "lwip_netconf.h"
#include "lwip/opt.h"
#include "lwip/sockets.h"
#include "lwip/dns.h"
#include "lwip/api.h"
#include "lwip/inet.h"
#include "lwip/inet_chksum.h"
#include "lwip/netdb.h"
#include "lwip/prot/icmp.h"
#include "lwip/prot/ip.h"
#include "lwip/prot/ip4.h"

#include "mbedtls/ssl.h"
#include "mbedtls/net_sockets.h"
#include "mbedtls/error.h"
#include "mbedtls/debug.h"

extern struct netif xnetif[NET_IF_NUM];

#ifdef __cplusplus
}
#endif

extern atcmd_cwstate_t cwstate;

uint8_t cip_mux = 0;            // indicator for single / multiple simultaneous connections mode
uint8_t cip_mode = 0;           // indicator for normal / passthrough transmission mode
uint8_t cip_dinfo = 0;          // indicator for hiding / showing host & port information in received data
uint8_t cip_recvmode = 0;       // indicator for active / passive data receiving mode
uint8_t cip_dnsmode = 0;        // indicator for auto / manual DNS configuration

uint16_t transmission_mode_len = 0;     // normal transmission mode data length to receive
uint8_t passthrough_mode = 0;           // indicator for passthrough mode currently active
uint32_t atcmd_tcpip_passthrough_last_tick = 0;
uint16_t cip_reconnintv = 1;

TaskHandle_t atcmd_tcpip_receive_handle = NULL;
TaskHandle_t atcmd_tcpip_passthrough_send_handle = NULL;
SemaphoreHandle_t atcmd_tcpip_tx_sema = NULL;

atcmd_client_conn_t client_conn_list[ATCMD_MAX_CLIENT_CONN];
uint8_t atcmd_tcpip_rx_buf[ATCMD_TCPIP_RX_BUFFER_SIZE];     // RX from network, sent to UART
uint8_t atcmd_tcpip_tx_buf[ATCMD_TCPIP_TX_BUFFER_SIZE];     // RX from UART, send to network
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

static unsigned int atcmd_tls_arc4random(void) {
    unsigned int res = xTaskGetTickCount();
    static unsigned int seed = 0xDEADB00B;

    seed = ((seed & 0x007F00FF) << 7) ^ 
        ((seed & 0x0F80FF00) >> 8) ^ // be sure to stir those low bits
        (res << 13) ^ (res >> 9);    // using the clock too!

    return seed;
}

static void atcmd_tls_get_random_bytes(void *buf, size_t len) {
    unsigned int ranbuf;
    unsigned int *lp;
    int i, count;
    count = len / sizeof(unsigned int);
    lp = (unsigned int *) buf;

    for (i = 0; i < count; i ++) {
        lp[i] = atcmd_tls_arc4random();
        len -= sizeof(unsigned int);
    }

    if (len > 0) {
        ranbuf = atcmd_tls_arc4random();
        memcpy(&lp[i], &ranbuf, len);
    }
}

static int atcmd_tls_random(void *p_rng, unsigned char *output, size_t output_len) {
    p_rng = p_rng;
    atcmd_tls_get_random_bytes(output, output_len);
    return 0;
}

static int atcmd_tls_verify(void *data, mbedtls_x509_crt *crt, int depth, uint32_t *flags) {
    char buf[1024];
    ((void)data);

    mbedtls_x509_crt_info(buf, (sizeof(buf) - 1), "", crt);

    if (MBEDTLS_DEBUG_LEVEL >= 3) {
        printf( "\nVerify requested for (Depth %d):\n", depth );
        printf( "%s", buf );

        if ((*flags) == 0)
            printf(" This certificate has no flags\n");
        else
        {
            mbedtls_x509_crt_verify_info(buf, sizeof( buf ), " ! ", *flags);
            printf("%s\n", buf);
        }
    }

    return(0);
}

static void* atcmd_tls_calloc(size_t nelements, size_t elementSize) {
    size_t size;
    void *ptr = NULL;

    size = nelements * elementSize;
    ptr = pvPortMalloc(size);

    if(ptr)
        memset(ptr, 0, size);

    return ptr;
}

static void atcmd_tls_debug(void *ctx, int level, const char *file, int line, const char *str) {
    const char *p, *basename;

    (void)ctx;     // Remove unused parameter warning
    // Extract basename from file
    for( p = basename = file; *p != '\0'; p++ )
        if( *p == '/' || *p == '\\' )
            basename = p + 1;

    printf("%s:%04d: |%d| %s", basename, line, level, str );

    if (MBEDTLS_EXPORT_KEY) {
        // Code to format and output TLS 1.2 secrets necessary for Wireshark decoding
        static uint8_t in_client_random = 0;
        static uint8_t in_master_secret = 0;
        static uint8_t hexdump_lines_to_process = 0;
        static uint8_t key_done = 0;
        static char out_string[200] = {0};
        if ((level == 3)&&(!key_done)) {
            if (strstr(str, "dumping 'client hello, random bytes'")) {
                in_client_random = 1;
                hexdump_lines_to_process = 2;
                strcpy(out_string, "CLIENT_RANDOM ");
                return;
            } else if (strstr(str, "dumping 'master secret'")) {
                in_master_secret = 1;
                hexdump_lines_to_process = 3;
                strcat(out_string, " ");
                return;
            } else if ((!in_client_random && !in_master_secret) || hexdump_lines_to_process == 0) {
                return;
            }

            // Parse "0000:  64 df 18 71 ca 4a 4b e4 63 87 2a ef 5f 29 ca ff  ..."
            str = strstr(str, ":  ");
            if (!str || strlen(str) < 3 + 3*16) {
                goto reset;         // not the expected hex buffer
            }
            str += 3;               // skip over ":  "

            // Process sequences of "hh "
            for (int i = 0; i < 3 * 16; i += 3) {
                char c1 = str[i], c2 = str[i + 1], c3 = str[i + 2];
                if ((('0' <= c1 && c1 <= '9') || ('a' <= c1 && c1 <= 'f')) &&
                    (('0' <= c2 && c2 <= '9') || ('a' <= c2 && c2 <= 'f')) &&
                    c3 == ' ') {
                    char str1[2] = {c1,0};
                    char str2[2] = {c2,0};
                    strcat(out_string, str1);
                    strcat(out_string, str2);
                } else {
                    goto reset;     // unexpected non-hex char
                }
            }

            if (--hexdump_lines_to_process != 0 || !in_master_secret) {
                return;             // line is not yet finished
            }

    reset:
            hexdump_lines_to_process = 0;
            in_client_random = in_master_secret = 0;
            key_done = 1;
            strcat(out_string, "\n");   // finish key log line
            printf("============== Wireshark TLS decryption key ==============\n");
            printf("%s", out_string);
            printf("==========================================================\n");
        }
    }
}

uint8_t atcmd_connection_close(uint8_t link_id) {
    // Set as inactive first to prevent duplicate close message from auto receive task
    client_conn_list[link_id].active = CONN_ROLE_INACTIVE;
    vTaskDelay(20/portTICK_PERIOD_MS);

    if (client_conn_list[link_id].protocol == CONN_MODE_TCP) {              // TCP case
        if (lwip_close(client_conn_list[link_id].sockfd) != 0) {
            return ATCMD_ERROR;
        }
        atcmd_init_conn_struct(&(client_conn_list[link_id]));
    } else if (client_conn_list[link_id].protocol == CONN_MODE_SSL) {       // SSL case
        mbedtls_ssl_context* ssl = client_conn_list[link_id].ssl;
        mbedtls_ssl_config* conf = (mbedtls_ssl_config*)(ssl->conf);
        mbedtls_net_context server_fd;
        server_fd.fd = client_conn_list[link_id].sockfd;

        mbedtls_ssl_close_notify(ssl);
        mbedtls_net_free(&server_fd);
        mbedtls_ssl_config_free(conf);
        free(conf);
        mbedtls_ssl_free(ssl);
        free(ssl);

        atcmd_init_conn_struct(&(client_conn_list[link_id]));
    } else {                                                                // UDP case
        if (lwip_close(client_conn_list[link_id].sockfd) != 0) {
            return ATCMD_ERROR;
        }
        atcmd_init_conn_struct(&(client_conn_list[link_id]));
    }
    return ATCMD_OK;
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
        uint16_t offset = 0;
        uint8_t retry = 1;

        while(retry) {
            ret += mbedtls_ssl_write(client_conn_list[link_id].ssl, &(atcmd_tcpip_tx_buf[offset]), send_size);

            if (ret >= send_size) {
                // All data sent, done
                retry = 0;
            } else if ((ret == MBEDTLS_ERR_SSL_WANT_WRITE) || (ret == MBEDTLS_ERR_SSL_WANT_READ)) {
                // Repeat write call with same params
                printf("MBEDTLS_ssl_write want write/read\r\n");
                retry = 1;
                vTaskDelay(5/portTICK_PERIOD_MS);
            } else if ((ret > 0) && (ret < send_size)) {
                // Partial data sent, retry sending remaining data
                printf("MBEDTLS_ssl_write partial write\r\n");
                send_size -= ret;
                offset += ret;
                retry = 1;
                vTaskDelay(5/portTICK_PERIOD_MS);
            } else if (ret <= 0) {
                printf("MBEDTLS_ssl_write error %d\r\n", ret);
                retry = 0;
            } else {
                printf("MBEDTLS_ssl_write unhandled error %d\r\n", ret);
                retry = 0;
            }
        }

    } else {                                                                // UDP case
        // initialize dest structure
        struct sockaddr_in serv_addr;
        memset(&serv_addr, 0, sizeof(serv_addr));
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_addr.s_addr = client_conn_list[link_id].addr;
        serv_addr.sin_port = htons(client_conn_list[link_id].port);

        ret = lwip_sendto(client_conn_list[link_id].sockfd, atcmd_tcpip_tx_buf, send_size, 0, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
        if(ret <= 0) {
            error_no = lwip_getsocklasterr(client_conn_list[link_id].sockfd);
            at_printf("[atcmd_tcpip_send_data] ERROR:Failed to send data ret = %d | error = %d\r\n", ret, error_no);
        }
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
int atcmd_tcpip_receive_data(uint8_t link_id, int* recv_size, int flags) {
    struct timeval tv;
    fd_set readfds;
    int error_no = 0, ret = 0, size = 0;

    // Receive data according to connection type
    if (client_conn_list[link_id].protocol == CONN_MODE_TCP) {      // TCP case
        // Check to see if socket has data to read
        FD_ZERO(&readfds);
        FD_SET(client_conn_list[link_id].sockfd, &readfds);
        tv.tv_sec = 0;
        tv.tv_usec = 20000;     // read timeout of 20ms
        ret = lwip_select(client_conn_list[link_id].sockfd + 1, &readfds, NULL, NULL, &tv);
        if (!( (ret > 0) && (FD_ISSET(client_conn_list[link_id].sockfd, &readfds)) )) {
            // No receive event
            *recv_size = size;
            return 0;
        }

        if (*recv_size > 0) {
            size = lwip_recv(client_conn_list[link_id].sockfd, atcmd_tcpip_rx_buf, *recv_size, flags);
        } else {
            size = lwip_recv(client_conn_list[link_id].sockfd, atcmd_tcpip_rx_buf, ATCMD_TCPIP_RX_BUFFER_SIZE, flags);
        }

        if (size == 0) {
            // ERROR:Connection closed
            error_no = 7;
        } else if (size < 0) {
            // ERROR:Failed to receive data
            error_no = 8;
        }

    } else if (client_conn_list[link_id].protocol == CONN_MODE_SSL) {       // SSL case
        if (flags & MSG_PEEK) {
            // MbedTLS does not support peek feature
            size = mbedtls_ssl_get_bytes_avail(client_conn_list[link_id].ssl);
            if (size == 0) {
                // No remaining bytes in MbedTLS buffer, check TCP socket for presence of encrypted bytes
                // assume there is data if TCP socket has data pending
                FD_ZERO(&readfds);
                FD_SET(client_conn_list[link_id].sockfd, &readfds);
                tv.tv_sec = 0;
                tv.tv_usec = 20000;     // read timeout of 20ms
                ret = lwip_select(client_conn_list[link_id].sockfd + 1, &readfds, NULL, NULL, &tv);
                if (!( (ret > 0) && (FD_ISSET(client_conn_list[link_id].sockfd, &readfds)) )) {
                    // No receive event
                    *recv_size = size;
                    return 0;
                }

                if (*recv_size > 0) {
                    size = lwip_recv(client_conn_list[link_id].sockfd, atcmd_tcpip_rx_buf, *recv_size, flags);
                } else {
                    size = lwip_recv(client_conn_list[link_id].sockfd, atcmd_tcpip_rx_buf, ATCMD_TCPIP_RX_BUFFER_SIZE, flags);
                }
            }
        } else {
            // Not peeking, use MbedTLS read API for data decryption
            if (*recv_size > 0) {
                size = mbedtls_ssl_read(client_conn_list[link_id].ssl, atcmd_tcpip_rx_buf, *recv_size);
            } else {
                size = mbedtls_ssl_read(client_conn_list[link_id].ssl, atcmd_tcpip_rx_buf, ATCMD_TCPIP_RX_BUFFER_SIZE);
            }
        }

        if (size == 0) {
            // ERROR:Connection closed
            error_no = 7;
        } else if (size < 0) {
            // ERROR:Failed to receive data / no data received / receive timeout
            if((size == MBEDTLS_ERR_SSL_WANT_READ || size == MBEDTLS_ERR_SSL_WANT_WRITE || size == MBEDTLS_ERR_NET_RECV_FAILED)) {
                size = 0;
            } else { // Other error code, close connection
                error_no = 8;
            }
        }

    } else {        // UDP case
        // Check to see if socket has data to read
        FD_ZERO(&readfds);
        FD_SET(client_conn_list[link_id].sockfd, &readfds);
        tv.tv_sec = 0;
        tv.tv_usec = 20000;     // read timeout of 20ms
        ret = lwip_select(client_conn_list[link_id].sockfd + 1, &readfds, NULL, NULL, &tv);
        if (!( (ret > 0) && (FD_ISSET(client_conn_list[link_id].sockfd, &readfds)) )) {
            // No receive event
            *recv_size = size;
            return 0;
        }

        struct sockaddr_in serv_addr;
        int addr_len = sizeof(serv_addr);
        memset(&serv_addr, 0, sizeof(serv_addr));
        serv_addr.sin_family = AF_INET;

        if (*recv_size > 0) {
            size = lwip_recvfrom(client_conn_list[link_id].sockfd, atcmd_tcpip_rx_buf, *recv_size, flags, (struct sockaddr*)&serv_addr, (socklen_t *)&addr_len);
        } else {
            size = lwip_recvfrom(client_conn_list[link_id].sockfd, atcmd_tcpip_rx_buf, ATCMD_TCPIP_RX_BUFFER_SIZE, flags, (struct sockaddr*)&serv_addr, (socklen_t *)&addr_len);
        }
        client_conn_list[link_id].remote_port = ntohs(serv_addr.sin_port);
        client_conn_list[link_id].remote_addr = serv_addr.sin_addr.s_addr;

        if (!cip_recvmode) {
            // From testing, target address does not update when in active receiving mode
            if (client_conn_list[link_id].udp_mode != 0) {
                // Update target address and port to received address and port
                client_conn_list[link_id].addr = client_conn_list[link_id].remote_addr;
                client_conn_list[link_id].port = client_conn_list[link_id].remote_port;
            }
            if (client_conn_list[link_id].udp_mode == 1) {
                // UDP mode 1 only updates once
                client_conn_list[link_id].udp_mode = 0;
            }
        }

        if (size < 0) {
            // ERROR:Failed to receive data
            error_no = 8;
        }
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
        atcmd_connection_close(link_id);
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
                if (cip_recvmode) {
                    if (passthrough_mode) {
                        // if in passthrough mode, receive and print data
                        error_no = atcmd_tcpip_receive_data(link_id, &recv_size, 0);
                        if ((error_no == 0) && recv_size) {
                            atcmd_tcpip_rx_buf[recv_size] = '\0';
                            at_print_data(atcmd_tcpip_rx_buf, recv_size);
                            vTaskDelay(20/portTICK_PERIOD_MS);
                        }
                    } else {
                        // peek at how much data is pending
                        if (client_conn_list[link_id].recv_data_len == 0) {
                            error_no = atcmd_tcpip_receive_data(link_id, &recv_size, MSG_PEEK);
                            if ((error_no == 0) && recv_size) {
                                at_printf("\r\n+IPD");
                                if (cip_mux) {
                                    at_printf(",%d", link_id);
                                }
                                at_printf(",%d", recv_size);
                                at_printf("\r\n");
                                client_conn_list[link_id].recv_data_len = recv_size;
                                vTaskDelay(20/portTICK_PERIOD_MS);
                            }
                        }
                    }
                } else {
                    // Not in passive data receive mode
                    error_no = atcmd_tcpip_receive_data(link_id, &recv_size, 0);
                    if ((error_no == 0) && recv_size) {
                        atcmd_tcpip_rx_buf[recv_size] = '\0';
                        if (!cip_mode) {
                            at_printf("\r\n+IPD");
                            if (cip_mux) {
                                at_printf(",%d", link_id);
                            }
                            at_printf(",%d", recv_size);
                            if (cip_dinfo) {
                                at_printf(",\"%s\"", inet_ntoa(client_conn_list[link_id].remote_addr));
                                at_printf(",%d", client_conn_list[link_id].remote_port);
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
    int sockfd = INVALID_SOCKET_ID;

    if (client_conn_list[link_id].protocol == CONN_MODE_TCP) {              // TCP case
        sockfd = lwip_socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (sockfd == INVALID_SOCKET_ID) {
            if (AT_DEBUG) printf("[atcmd_client_start] Failed to create socket\r\n");
            return ATCMD_ERROR;
        }

        client_conn_list[link_id].sockfd = sockfd;

        // initialize dest structure
        struct sockaddr_in serv_addr;
        memset(&serv_addr, 0, sizeof(serv_addr));
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_addr.s_addr = client_conn_list[link_id].addr;
        serv_addr.sin_port = htons(client_conn_list[link_id].port);

        if (lwip_connect(sockfd, ((struct sockaddr*)&serv_addr), sizeof(serv_addr)) != 0) {
            // Initial TCP connection failed, teardown connection
            lwip_close(sockfd);
            atcmd_init_conn_struct(&(client_conn_list[link_id]));
            return ATCMD_ERROR;
        }

        // Initial TCP connection successful, config TCP options for socket
        int enable = 1;
        int disable = 0;
        int timeout = 100;     // 100 msecs

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
        client_conn_list[link_id].remote_addr = client_conn_list[link_id].addr;
        client_conn_list[link_id].remote_port = client_conn_list[link_id].port;

    } else if (client_conn_list[link_id].protocol == CONN_MODE_SSL) {       // SSL case
        sockfd = lwip_socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (sockfd == INVALID_SOCKET_ID) {
            if (AT_DEBUG) printf("[atcmd_client_start] Failed to create socket\r\n");
            return ATCMD_ERROR;
        }

        client_conn_list[link_id].sockfd = sockfd;

        // initialize dest structure
        struct sockaddr_in serv_addr;
        memset(&serv_addr, 0, sizeof(serv_addr));
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_addr.s_addr = client_conn_list[link_id].addr;
        serv_addr.sin_port = htons(client_conn_list[link_id].port);

        if (lwip_connect(sockfd, ((struct sockaddr*)&serv_addr), sizeof(serv_addr)) != 0) {
            // Initial TCP connection failed, teardown connection
            lwip_close(sockfd);
            atcmd_init_conn_struct(&(client_conn_list[link_id]));
            return ATCMD_ERROR;
        }

        // Initial TCP connection successful, config TCP options for socket
        int enable = 1;
        int disable = 0;
        int timeout = 100;     // 100 msecs

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

        // Setup TLS connection
        int ret;
        mbedtls_ssl_context* ssl = NULL;
        mbedtls_ssl_config* conf = NULL;
        mbedtls_x509_crt* ca_cert = NULL;
        mbedtls_x509_crt* client_cert = NULL;
        mbedtls_pk_context* client_key = NULL;
        do {
            // Setup mbedTLS configurations for TLS connection
            mbedtls_platform_set_calloc_free(atcmd_tls_calloc,vPortFree);
            ssl = (mbedtls_ssl_context*)malloc(sizeof(mbedtls_ssl_context));
            conf = (mbedtls_ssl_config*)malloc(sizeof(mbedtls_ssl_config));
            if ((ssl == NULL) || (conf == NULL)) {
                printf("ERROR: mbedtls malloc failed! \r\n");
                ret = -1;
                break;
            }

            mbedtls_ssl_init(ssl);
            mbedtls_ssl_config_init(conf);

            if (MBEDTLS_DEBUG_LEVEL > 0) {
                mbedtls_ssl_conf_verify(conf, atcmd_tls_verify, NULL);
                mbedtls_ssl_conf_dbg(conf, atcmd_tls_debug, NULL);
                mbedtls_debug_set_threshold(MBEDTLS_DEBUG_LEVEL);
            }

            mbedtls_ssl_set_bio(ssl, &(client_conn_list[link_id].sockfd), mbedtls_net_send, mbedtls_net_recv, NULL);

            if (strlen(client_conn_list[link_id].ssl_cli_sni)) {
                mbedtls_ssl_set_hostname(ssl, (client_conn_list[link_id].ssl_cli_sni));
            }

            if((mbedtls_ssl_config_defaults(conf, MBEDTLS_SSL_IS_CLIENT, MBEDTLS_SSL_TRANSPORT_STREAM, MBEDTLS_SSL_PRESET_DEFAULT)) != 0) {
                printf("ERROR: mbedtls ssl config defaults failed! \r\n");
                ret = -1;
                break;
            }
            mbedtls_ssl_conf_rng(conf, atcmd_tls_random, NULL);

            if (clientCA != NULL) {
                // Configure mbedTLS to use certificate authentication method
                ca_cert = (mbedtls_x509_crt*)mbedtls_calloc(sizeof(mbedtls_x509_crt), 1);
                mbedtls_x509_crt_init(ca_cert);
                if (mbedtls_x509_crt_parse(ca_cert, clientCA, strlen((char*)clientCA)+1) != 0) {
                    printf("ERROR: mbedtls x509 crt parse failed! \r\n");
                    ret = -1;
                    break;
                }
                mbedtls_ssl_conf_ca_chain(conf, ca_cert, NULL);
                mbedtls_ssl_conf_authmode(conf, MBEDTLS_SSL_VERIFY_REQUIRED);
            } else {
                mbedtls_ssl_conf_authmode(conf, MBEDTLS_SSL_VERIFY_NONE);
            }

            if ((clientCert != NULL) && (clientKey != NULL)) {
                client_cert = (mbedtls_x509_crt*)mbedtls_calloc(sizeof(mbedtls_x509_crt), 1);
                client_key = (mbedtls_pk_context*)mbedtls_calloc(sizeof(mbedtls_pk_context), 1);
                if ((client_cert == NULL) || (client_key == NULL)) {
                    printf("ERROR: malloc client_crt or client_rsa failed! \r\n");
                    ret = -1;
                    break;
                }

                mbedtls_x509_crt_init(client_cert);
                mbedtls_pk_init(client_key);

                if (mbedtls_x509_crt_parse(client_cert, clientCert, strlen((char*)clientCert)+1) != 0) {
                    printf("ERROR: mbedtls x509 parse client_crt failed! \r\n");
                    ret = -1;
                    break;
                }

                if (mbedtls_pk_parse_key(client_key, clientKey, strlen((char*)clientKey)+1, NULL, 0) != 0) {
                    printf("ERROR: mbedtls x509 parse client_rsa failed! \r\n");
                    ret = -1;
                    break;
                }
                mbedtls_ssl_conf_own_cert(conf, client_cert, client_key);
            }

            if((mbedtls_ssl_setup(ssl, conf)) != 0) {
                printf("ERROR: mbedtls ssl setup failed!\r\n");
                ret = -1;
                break;
            }

            ret = mbedtls_ssl_handshake(ssl);
            if (ret < 0) {
                printf("ERROR: mbedtls ssl handshake failed: -0x%04X \r\n", -ret);
                ret = -1;
                break;
            }

        }while (0);

        if (client_key) {
            mbedtls_pk_free(client_key);
            mbedtls_free(client_key);
            client_key = NULL;
        }
        if (client_cert) {
            mbedtls_x509_crt_free(client_cert);
            mbedtls_free(client_cert);
            client_cert = NULL;
        }
        if (ca_cert) {
            mbedtls_x509_crt_free(ca_cert);
            mbedtls_free(ca_cert);
            ca_cert = NULL;
        }

        if (ret < 0) {
            // TLS connection failed somewhere, teardown and free resources
            if (ssl != NULL) {
                mbedtls_ssl_free(ssl);
                free(ssl);
            }
            if (conf != NULL) {
                mbedtls_ssl_config_free(conf);
                free(conf);
            }
            lwip_close(sockfd);
            atcmd_init_conn_struct(&(client_conn_list[link_id]));
            return ATCMD_ERROR;
        } else {
            // Connection successful
            client_conn_list[link_id].ssl = ssl;
            client_conn_list[link_id].active = CONN_ROLE_CLIENT;
            client_conn_list[link_id].remote_addr = client_conn_list[link_id].addr;
            client_conn_list[link_id].remote_port = client_conn_list[link_id].port;
        }

    } else {                                                                // UDP case
        sockfd = lwip_socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
        if (sockfd == INVALID_SOCKET_ID) {
            if (AT_DEBUG) printf("[atcmd_client_start] Failed to create socket\r\n");
            return ATCMD_ERROR;
        }

        client_conn_list[link_id].sockfd = sockfd;

        // initialize dest structure
        struct sockaddr_in serv_addr;
        memset(&serv_addr, 0, sizeof(serv_addr));
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_addr.s_addr = INADDR_ANY;
        if (client_conn_list[link_id].local_port) {
            serv_addr.sin_port = htons(client_conn_list[link_id].local_port);
        }

        if (bind(sockfd, ((struct sockaddr *)&serv_addr), sizeof(serv_addr)) < 0) {
            if (AT_DEBUG) printf("[atcmd_client_start] ERROR on binding\r\n");
            lwip_close(sockfd);
            atcmd_init_conn_struct(&(client_conn_list[link_id]));
            return ATCMD_ERROR;
        }

//        // Initial UDP bind successful, config UDP options for socket
        int timeout = 100;     // 100 msecs

        int ca_len = sizeof(serv_addr);
        if (lwip_getsockname(sockfd, ((struct sockaddr*)&serv_addr), (socklen_t *)&ca_len) == 0) {
            client_conn_list[link_id].local_port = ntohs(serv_addr.sin_port);
        } else {
            lwip_close(sockfd);
            atcmd_init_conn_struct(&(client_conn_list[link_id]));
            return ATCMD_ERROR;
        }
        lwip_setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
        lwip_setsockopt(sockfd, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout));

        client_conn_list[link_id].active = CONN_ROLE_CLIENT;

    }

    // Connection established, start task to listen and process incoming data
    if (atcmd_tcpip_receive_handle == NULL) {
        int result;
        result = xTaskCreate(atcmd_tcpip_receive_task, "atcmd_lwip_rx_task", 1024, NULL, tskIDLE_PRIORITY + 3, &atcmd_tcpip_receive_handle);
        if (result != pdPASS) {
            if (AT_DEBUG) printf("[atcmd_client_start] atcmd_lwip_rx_task create failed\r\n");
            atcmd_connection_close(link_id);
            return ATCMD_ERROR;
        }
    }
    if (!passthrough_mode) {
        if (cip_mux) {
            at_printf("%d,", link_id);
        }
        at_printf("CONNECT\r\n");
    }

    return ATCMD_OK;
}

//---------------------------- Commands for TCP/IP ATCMD functionality ----------------------------//

uint8_t e_AT_CIPSTATUS(void *arg) {
    // Obtain the TCP/UDP/SSL Connection Status and Information
    // AT+CIPSTATUS
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
    (void)arg;
    uint8_t argc = 0;
    char* argv[ATCMD_MAX_ARG_CNT] = {0};
    char* domain = NULL;      // Arg 1
    uint8_t ip_network = 0;
    ip_addr_t ip_addr;

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

    if (inet_aton(domain, &ip_addr.u_addr.ip4)) {
        // valid IP address provided, return same address
        at_printf("+CIPDOMAIN:\"%s\"\r\n", inet_ntoa(ip_addr.u_addr.ip4));
    } else {
        // Not a valid IP address provided, attempt to DNS resolve hostname
        err_t err;
        err = netconn_gethostbyname_addrtype(domain, &ip_addr, NETCONN_DNS_IPV4);
        if (err != ERR_OK) {
            if (AT_DEBUG) printf("netconn_gethostbyname_addrtype error: %d\r\n", err);
            return ATCMD_ERROR;
        } else {
            at_printf("+CIPDOMAIN:\"%s\"\r\n", inet_ntoa(ip_addr.u_addr));
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

    // Cannot start new connection if passthrough mode is enabled and a connection already exist
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
            conn_mode = CONN_MODE_UDP;
        } else if (strcmp(conn_type, "SSL") == 0) {
            conn_mode = CONN_MODE_SSL;
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
        // 0: After UDP data is received, the parameters <"remote host"> and <remote port> will stay unchanged (default).
        // 1: Only the first time that UDP data is received from an IP address and port that are different from the initially set value of parameters <remote host> and <remote port>, will they be changed to the IP address and port of the device that sends the data.
        // 2: Each time UDP data is received, the <"remote host"> and <remote port> will be changed to the IP address and port of the device that sends the data.
        if (argv[5 + cip_mux] != NULL) {
            udp_mode = atoi(argv[5 + cip_mux]);
            if (udp_mode > 2) {
                return ATCMD_ERROR;
            }
            client_conn_list[link_id].udp_mode = udp_mode;
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
        if ((argc < 3) || (argc > 5)) {
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
        if (client_conn_list[link_id].protocol != CONN_MODE_UDP) {
            // Remote host only valid for UDP connection
            return ATCMD_ERROR;
        }
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
    }

    // Remote Port
    if (argv[3 + cip_mux] != NULL) {
        if (client_conn_list[link_id].protocol != CONN_MODE_UDP) {
            // Remote port only valid for UDP connection
            return ATCMD_ERROR;
        }
        remote_port = atoi(argv[3 + cip_mux]);
        if (remote_port < 0 || remote_port > 65535) {
            return ATCMD_ERROR;
        }
        client_conn_list[link_id].port = remote_port;
    }

    int ret = 0;
    // Enter normal transmission mode
    atcmd_tcpip_tx_buflen = 0;
    transmission_mode_len = data_len;
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
    // AT+CIPCLOSE=<link ID>
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

    if (atcmd_connection_close(link_id) != ATCMD_OK) {
        return ATCMD_ERROR;
    }
    at_printf("%d,CLOSED\r\n", link_id);
    return ATCMD_OK;
}

uint8_t e_AT_CIPCLOSE(void *arg) {
    // Close TCP/UDP/SSL connection in the single connection mode.
    // AT+CIPCLOSE
    (void)arg;

    if (cip_mux == 1) {
        return ATCMD_ERROR;
    }
    if (client_conn_list[0].active == CONN_ROLE_INACTIVE) {
        return ATCMD_ERROR;
    }

    if (atcmd_connection_close(0) != ATCMD_OK) {
        return ATCMD_ERROR;
    }
    at_printf("CLOSED\r\n");
    return ATCMD_OK;
}

uint8_t e_AT_CIFSR(void *arg) {
    // Obtain the Local IP Address and MAC Address
    // AT+CIFSR
    (void)arg;
    //<”APIP”>: IPv4 address of Wi-Fi softAP interface
    //<”APIP6LL”>: Linklocal IPv6 address of Wi-Fi softAP interface
    //<”APIP6GL”>: Global IPv6 address of Wi-Fi softAP interface
    //<”APMAC”>: MAC address of Wi-Fi softAP interface

    //<”STAIP”>: IPv4 address of Wi-Fi station interface
    //<”STAIP6LL”>: Linklocal IPv6 address of Wi-Fi station interface
    //<”STAIP6GL”>: Global IPv6 address of Wi-Fi station interface
    //<”STAMAC”>: MAC address of Wi-Fi station interface

    extern rtw_mode_t cw_mode;
    uint8_t* mac = LwIP_GetMAC(&xnetif[0]);
    uint8_t* ip = LwIP_GetIP(&xnetif[0]);
    
    if (cw_mode == RTW_MODE_AP) {
        at_printf("+CIFSR:APIP,\"%d.%d.%d.%d\"\r\n", ip[0], ip[1], ip[2], ip[3]);
        at_printf("+CIFSR:APMAC,\"%02x:%02x:%02x:%02x:%02x:%02x\"\r\n", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    } else if (cw_mode == RTW_MODE_STA) {
        if (cwstate != CWSTATE_CONNECTEDIP) {
            at_printf("+CIFSR:STAIP,\"%d.%d.%d.%d\"\r\n", 0, 0, 0, 0);
        } else {
            at_printf("+CIFSR:STAIP,\"%d.%d.%d.%d\"\r\n", ip[0], ip[1], ip[2], ip[3]);
        }
        at_printf("+CIFSR:STAMAC,\"%02x:%02x:%02x:%02x:%02x:%02x\"\r\n", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    } else if (cw_mode == RTW_MODE_STA_AP) {
        uint8_t* ap_mac = LwIP_GetMAC(&xnetif[1]);
        uint8_t* ap_ip = LwIP_GetIP(&xnetif[1]);

        at_printf("+CIFSR:APIP,\"%d.%d.%d.%d\"\r\n", ap_ip[0], ap_ip[1], ap_ip[2], ap_ip[3]);
        at_printf("+CIFSR:APMAC,\"%02x:%02x:%02x:%02x:%02x:%02x\"\r\n", ap_mac[0], ap_mac[1], ap_mac[2], ap_mac[3], ap_mac[4], ap_mac[5]);
        if (cwstate != CWSTATE_CONNECTEDIP) {
            at_printf("+CIFSR:STAIP,\"%d.%d.%d.%d\"\r\n", 0, 0, 0, 0);
        } else {
            at_printf("+CIFSR:STAIP,\"%d.%d.%d.%d\"\r\n", ip[0], ip[1], ip[2], ip[3]);
        }
        at_printf("+CIFSR:STAMAC,\"%02x:%02x:%02x:%02x:%02x:%02x\"\r\n", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    }
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

uint8_t q_AT_CIPSSLCSNI(void *arg) {
    // Query the SNI configuration of each connection.
    // AT+CIPSSLCSNI?
    (void)arg;

    for (uint8_t link_id = 0; link_id < ATCMD_MAX_CLIENT_CONN; link_id++) {
        if (strlen(client_conn_list[link_id].ssl_cli_sni)) {
            at_printf("+CIPSSLCSNI:%d,\"%s\"\r\n", link_id, client_conn_list[link_id].ssl_cli_sni);
        }
    }
    return ATCMD_OK;
}

uint8_t s_AT_CIPSSLCSNI(void *arg) {
    // Set the SNI configuration of each connection.
    // Single connection: (AT+CIPMUX=0)
        // AT+CIPSSLCSNI=<"sni">
    // Multiple connections: (AT+CIPMUX=1)
        // AT+CIPSSLCSNI=<link ID>,<"sni">
    (void)arg;
    uint8_t argc = 0;
    char* argv[ATCMD_MAX_ARG_CNT] = {0};
    uint8_t link_id = 0;          // Arg 1
    char* sni = NULL;             // Arg 2

    if (!arg) {
        return ATCMD_ERROR;
    }
    argc = atcmd_parse_params((char*)arg, argv);
    if (cip_mux) {
        if (argc != 3) {
            return ATCMD_ERROR;
        }
    } else {
        if (argc != 2) {
            return ATCMD_ERROR;
        }
    }

    // Link ID
    if (cip_mux) {
        if (argv[1] != NULL) {
            link_id = atoi(argv[1]);
            if (link_id >= ATCMD_MAX_CLIENT_CONN + 1) {
                return ATCMD_ERROR;
            }
        } else {
            return ATCMD_ERROR;
        }
    }

    // Server Name Indication
    if (argv[1 + cip_mux] != NULL) {
        sni = argv[1 + cip_mux];
        if (strlen(sni) > 64) {
            return ATCMD_ERROR;
        }
    } else {
        return ATCMD_ERROR;
    }

    if (link_id == ATCMD_MAX_CLIENT_CONN) {     // Set SNI for all connections
        for (link_id = 0; link_id < ATCMD_MAX_CLIENT_CONN; link_id++) {
            strcpy(client_conn_list[link_id].ssl_cli_sni, sni);
        }
    } else {                                    // Set SNI for specified connection
        strcpy(client_conn_list[link_id].ssl_cli_sni, sni);
    }

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
    // Set the automatic reconnecting interval when TCP/SSL transmission breaks in the Wi-Fi Passthrough Mode.
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

uint8_t q_AT_CIPRECVMODE(void *arg) {
    // Query the socket receiving mode.
    // AT+CIPRECVMODE?
    (void)arg;
    at_printf("+CIPRECVMODE:%d\r\n", cip_recvmode);
    return ATCMD_OK;
}

uint8_t s_AT_CIPRECVMODE(void *arg) {
    // Set the socket receiving mode.
    // AT+CIPRECVMODE=<mode>
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
    cip_recvmode = mode;
    if (cip_recvmode == 0) {
        // zero out pending data counts for all connections
        for (uint8_t i = 0; i < ATCMD_MAX_CLIENT_CONN; i++) {
            client_conn_list[i].recv_data_len = 0;
        }
    }
    return ATCMD_OK;
}

uint8_t s_AT_CIPRECVDATA(void *arg) {
    // Obtain Socket Data in Passive Receiving Mode
    // Single connection: (AT+CIPMUX=0)
        // AT+CIPRECVDATA=<len>
    // Multiple connections: (AT+CIPMUX=1)
        // AT+CIPRECVDATA=<link_id>,<len>
    (void)arg;
    uint8_t argc = 0;
    char* argv[ATCMD_MAX_ARG_CNT] = {0};
    uint8_t link_id = 0;        // Arg 1
    int data_len = 0;       // Arg 2
    int error_no = 0;

    if (cip_recvmode == 0) {
        return ATCMD_ERROR;
    }
    if (!arg) {
        return ATCMD_ERROR;
    }
    argc = atcmd_parse_params((char*)arg, argv);
    if (cip_mux) {
        if (argc != 3) {
            return ATCMD_ERROR;
        }
    } else {
        if (argc != 2) {
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

    // Data length
    if (argv[1 + cip_mux] != NULL) {
        data_len = atoi(argv[1 + cip_mux]);
        if ((data_len > ATCMD_TCPIP_TX_BUFFER_SIZE) || (data_len <= 0)) {
            return ATCMD_ERROR;
        }
    } else {
        return ATCMD_ERROR;
    }

    // Check for data pending read
    if (client_conn_list[link_id].recv_data_len == 0) {
        return ATCMD_ERROR;
    }

    error_no = atcmd_tcpip_receive_data(link_id, &data_len, 0);
    if ((error_no == 0) && data_len) {
        atcmd_tcpip_rx_buf[data_len] = '\0';
        at_printf("+CIPRECVDATA:");
        at_printf("%d,", data_len);
        if (cip_dinfo) {
            at_printf("\"%s\",", inet_ntoa(client_conn_list[link_id].remote_addr));
            at_printf("%d,", client_conn_list[link_id].remote_port);
        }
        at_print_data(atcmd_tcpip_rx_buf, data_len);
        // Allow data in UART buffer to clear
        vTaskDelay(20/portTICK_PERIOD_MS);
        // set to zero to trigger new pending data message
        client_conn_list[link_id].recv_data_len = 0;
    } else {
        return ATCMD_ERROR;
    }

    return ATCMD_OK;
}

uint8_t q_AT_CIPRECVLEN(void *arg) {
    // Query the length of the entire data buffered for the connection.
    // AT+CIPRECVLEN?
    (void)arg;

    uint8_t conn_count = 0;
    if (cip_mux) {
        conn_count = ATCMD_MAX_CLIENT_CONN;
    } else {
        conn_count = 1;
    }

    at_printf("+CIPRECVLEN:");
    for (uint8_t link_id = 0; link_id < conn_count; link_id++) {
        if (client_conn_list[link_id].active == CONN_ROLE_INACTIVE) {
            at_printf("-1");
        } else {
            int error_no = 0;
            int recv_size = 0;
            // peek at how much data is pending
            error_no = atcmd_tcpip_receive_data(link_id, &recv_size, MSG_PEEK);
            if ((error_no == 0) && recv_size) {
                client_conn_list[link_id].recv_data_len = recv_size;
                vTaskDelay(20/portTICK_PERIOD_MS);
            }
            at_printf("%d", client_conn_list[link_id].recv_data_len);
        }
        if (link_id < (conn_count - 1)) {
            at_printf(",");
        }
    }
    at_printf("\r\n");
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

uint8_t q_AT_CIPDNS(void *arg) {
    // Query the current DNS server information.
    // AT+CIPDNS?
    (void)arg;

    const ip_addr_t* dns0 = dns_getserver(0);
    const ip_addr_t* dns1 = dns_getserver(1);

    at_printf("+CIPDNS:%d,\"%s\"", cip_dnsmode, inet_ntoa(dns0->u_addr.ip4));
    at_printf(",\"%s\"\r\n", inet_ntoa(dns1->u_addr.ip4));

    return ATCMD_OK;
}

uint8_t s_AT_CIPDNS(void *arg) {
    // Set DNS server information.
    // AT+CIPDNS=<enable>[,<"DNS IP1">,<"DNS IP2">,<"DNS IP3">]
    (void)arg;
    uint8_t argc = 0;
    char* argv[ATCMD_MAX_ARG_CNT] = {0};
    uint8_t dns_mode = 0;
    ip_addr_t dns1_addr;
    ip_addr_t dns2_addr;

    if (!arg) {
        return ATCMD_ERROR;
    }
    argc = atcmd_parse_params((char*)arg, argv);
    if ((argc < 2) || (argc > 5)) {
        return ATCMD_ERROR;
    }

    dns_mode = atoi(argv[1]);
    if (dns_mode > 1) {
        return ATCMD_ERROR;
    }
    cip_dnsmode = dns_mode;

    if (dns_mode) {
        if (argv[2] != NULL) {
            if (inet_aton(argv[2], &dns1_addr.u_addr.ip4) == 0) {
                return ATCMD_ERROR;
            }
            if (dns1_addr.u_addr.ip4.addr == 0) {
                return ATCMD_ERROR;
            }
        } else {
            inet_aton(DEFAULT_DNS_1, &dns1_addr.u_addr.ip4);
        }
        if (argv[3] != NULL) {
            if (inet_aton(argv[3], &dns2_addr.u_addr.ip4) == 0) {
                return ATCMD_ERROR;
            }
            if (dns2_addr.u_addr.ip4.addr == 0) {
                return ATCMD_ERROR;
            }
        } else {
            inet_aton(DEFAULT_DNS_2, &dns2_addr.u_addr.ip4);
        }
    } else {
        inet_aton(DEFAULT_DNS_1, &dns1_addr.u_addr.ip4);
        inet_aton(DEFAULT_DNS_2, &dns2_addr.u_addr.ip4);
    }

    dns_setserver(0, &dns1_addr);
    dns_setserver(1, &dns2_addr);

    return ATCMD_OK;
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
      {"AT+CIPSSLCSNI",     NULL,   q_AT_CIPSSLCSNI,    s_AT_CIPSSLCSNI,    NULL,             {NULL, NULL}},
      {"AT+CIPRECONNINTV",  NULL,   q_AT_CIPRECONNINTV, s_AT_CIPRECONNINTV, NULL,             {NULL, NULL}},
      {"AT+CIPRECVMODE",    NULL,   q_AT_CIPRECVMODE,   s_AT_CIPRECVMODE,   NULL,             {NULL, NULL}},
      {"AT+CIPRECVDATA",    NULL,   NULL,               s_AT_CIPRECVDATA,   NULL,             {NULL, NULL}},
      {"AT+CIPRECVLEN",     NULL,   q_AT_CIPRECVLEN,    NULL,               NULL,             {NULL, NULL}},
      {"AT+PING",           NULL,   NULL,               s_AT_PING,          NULL,             {NULL, NULL}},
      {"AT+CIPDNS",         NULL,   q_AT_CIPDNS,        s_AT_CIPDNS,        NULL,             {NULL, NULL}},
};

void atcmd_tcpip_register(void) {
    atcmd_add_cmdtable(atcmd_tcpip_commands, sizeof(atcmd_tcpip_commands)/sizeof(atcmd_tcpip_commands[0]));
}
