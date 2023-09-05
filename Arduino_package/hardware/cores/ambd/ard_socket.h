#ifndef ARD_SOCKET_H
#define ARD_SOCKET_H
#include "main.h"

int set_nonblocking(int fd);

int start_server(uint16_t port, uint8_t protMode);

int start_server_v6(uint16_t port, uint8_t protMode);

int start_client(uint32_t ipAddress, uint16_t port, uint8_t protMode);

int start_clientv6(uint32_t *ipv6Address, uint16_t port, uint8_t protMode);

int start_client_v6(char ipv6Address[], uint16_t port, uint8_t protMode);

int get_sock_errno(int sock);

int set_sock_recv_timeout(int sock, int timeout);

void close_socket(int sock);

int enable_ipv6(void);

int get_ipv6_status(void);

// TCP
int sock_listen(int sock, int max);

int get_available(int sock);

int get_available_v6(int sock);

int recv_data(int sock, const uint8_t *data, uint16_t len, int flag);

int send_data(int sock, const uint8_t *data, uint16_t len, int flag);
// UDP
int get_receive(int sock, uint8_t *data, int length, int flag, uint32_t *peer_addr, uint16_t *peer_port);

int get_receive_v6(int server_fd, void *recv_data, int len, int flags, uint32_t *peer_addr, uint16_t *peer_port);

int sendto_data(int sock, const uint8_t *data, uint16_t len, uint32_t peer_ip, uint16_t peer_port);

int sendto_data_v6(int sock, const void *send_data, size_t len, uint32_t peer_ip, uint16_t peer_port);

void ipv6_udp_server(void);

// OTA

int ota_bind_socket(int server_socket, int OTAport);

void ota_listen_socket(int server_socket, int OTAport);

int ota_accept_socket(int server_socket);
#endif
