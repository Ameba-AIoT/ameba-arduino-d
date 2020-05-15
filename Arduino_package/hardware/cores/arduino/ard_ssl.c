#include "Arduino.h"
#include <sockets.h> 
#include <lwip/netif.h>
#include <polarssl/ssl.h>
#include <polarssl/memory.h>
#include <polarssl/net.h>
#include <polarssl/error.h>

#include "ard_ssl.h"

static unsigned int ard_ssl_arc4random(void)
{
    unsigned int res = xTaskGetTickCount();
    static unsigned int seed = 0xDEADB00B;

    seed = ((seed & 0x007F00FF) << 7) ^ 
        ((seed & 0x0F80FF00) >> 8) ^ // be sure to stir those low bits
        (res << 13) ^ (res >> 9);    // using the clock too!

    return seed;
}

static void get_random_bytes(void *buf, size_t len)
{
    unsigned int ranbuf;
    unsigned int *lp;
    int i, count;
    count = len / sizeof(unsigned int);
    lp = (unsigned int *) buf;

    for (i = 0; i < count; i ++) {
        lp[i] = ard_ssl_arc4random();
        len -= sizeof(unsigned int);
    }

    if (len > 0) {
        ranbuf = ard_ssl_arc4random();
        memcpy(&lp[i], &ranbuf, len);
    }
}

static int my_random(void *p_rng, unsigned char *output, size_t output_len)
{
    p_rng = p_rng;
    get_random_bytes(output, output_len);
    return 0;
}

static int my_verify(void *data, x509_crt *crt, int depth, int *flags) 
{
    char *buf;
    ((void)data);

    printf("\nVerify requested for (Depth %d):\n", depth);

    buf = (char *)(malloc(1024));
    if (buf == NULL) {
        return -1;
    }
    x509_crt_info(buf, (sizeof(buf) - 1), "", crt);
    printf("%s", buf);
    free(buf);

    if (((*flags) & BADCERT_EXPIRED) != 0) {
        printf("! server certificate has expired\n");
    }

    if (((*flags) & BADCERT_REVOKED) != 0) {
        printf("! server certificate has been revoked\n");
    }

    if (((*flags) & BADCERT_CN_MISMATCH) != 0) {
        printf("! CN mismatch\n");
    }

    if (((*flags) & BADCERT_NOT_TRUSTED) != 0) {
        printf("! self-signed or not signed by a trusted CA\n");
    }

    if (((*flags) & BADCRL_NOT_TRUSTED) != 0) {
        printf("! CRL not trusted\n");
    }

    if (((*flags) & BADCRL_EXPIRED) != 0) {
        printf("! CRL expired\n");
    }

    if (((*flags) & BADCERT_OTHER) != 0) {
        printf("! other (unknown) flag\n");
    }

    if ((*flags) == 0) {
        printf("This certificate has no flags\n");
    }

    return(0);
}

int start_ssl_client(sslclient_context *ssl_client, uint32_t ipAddress, uint32_t port, unsigned char* rootCABuff, unsigned char* cli_cert, unsigned char* cli_key)
{
    int ret = 0;
    int timeout;
    int enable = 1;
    x509_crt* cacert = NULL;
    static x509_crt* _cli_crt = NULL;
    static pk_context* _clikey_rsa = NULL;

    do {
        ssl_client->socket = -1;
        ssl_client->ssl = (ssl_context *)malloc(sizeof(ssl_context));
        if (ssl_client->ssl == NULL) {
            printf("\r\nERROR: malloc ssl context fail\n");
            ret = -1;
            break;
        }
        memset(ssl_client->ssl, 0, sizeof(ssl_context));
        memory_set_own(malloc, free);
        ssl_client->socket = lwip_socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (ssl_client->socket < 0) {
            printf("\r\nERROR opening socket\r\n");
            ret = -1;
            break;
        }

        struct sockaddr_in serv_addr;
        memset(&serv_addr, 0, sizeof(serv_addr));
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_addr.s_addr = ipAddress;
        serv_addr.sin_port = htons(port);

        if (lwip_connect(ssl_client->socket, ((struct sockaddr *)&serv_addr), sizeof(serv_addr)) == 0) {
            timeout = ssl_client->recvTimeout;
            lwip_setsockopt(ssl_client->socket, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
            timeout = 30000;
            lwip_setsockopt(ssl_client->socket, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout));
            lwip_setsockopt(ssl_client->socket, IPPROTO_TCP, TCP_NODELAY, &enable, sizeof(enable));
            lwip_setsockopt(ssl_client->socket, SOL_SOCKET, SO_KEEPALIVE, &enable, sizeof(enable));
        } else {
            printf("\r\nConnect to Server failed!\r\n");
            ret = -1;
            break;
        }

        if (ssl_init(ssl_client->ssl) != 0) {
            printf("\r\n init ssl failed\r\n");
            ret = -1;
            break;
        }

        ssl_set_endpoint(ssl_client->ssl, SSL_IS_CLIENT);

        if (rootCABuff != NULL) {
            cacert = polarssl_malloc(sizeof(x509_crt));
            x509_crt_init(cacert);
            ssl_set_authmode(ssl_client->ssl, SSL_VERIFY_REQUIRED);
            x509_crt_parse(cacert, rootCABuff, strlen((char*)rootCABuff));
            ssl_set_ca_chain(ssl_client->ssl, cacert, NULL, NULL);
            ssl_set_verify(ssl_client->ssl, my_verify, NULL);
        } else {
            ssl_set_authmode(ssl_client->ssl, SSL_VERIFY_NONE);
        }

        if ((cli_cert != NULL) && (cli_key != NULL)) {
            _cli_crt = polarssl_malloc(sizeof(x509_crt));
            if (_cli_crt != NULL) {
                x509_crt_init(_cli_crt);
            } else {
                ret = -1;
                break;
            }

            _clikey_rsa = polarssl_malloc(sizeof(pk_context));
            if (_clikey_rsa != NULL) {
                pk_init(_clikey_rsa);
            } else {
                ret = -1;
                break;
            }

            if (x509_crt_parse(_cli_crt, cli_cert, strlen((char*)cli_cert)) != 0) {
                ret = -1;
                break;
            }

            if (pk_parse_key(_clikey_rsa, cli_key, strlen((char*)cli_key), NULL, 0) != 0) {
                ret = -1;
                break;
            }

            ssl_set_own_cert(ssl_client->ssl, _cli_crt, _clikey_rsa);
        }

        ssl_set_rng(ssl_client->ssl, my_random, NULL);
        ssl_set_bio(ssl_client->ssl, net_recv, &ssl_client->socket, net_send, &ssl_client->socket);

        if ((ret = ssl_handshake(ssl_client->ssl)) != 0) {
            printf("ERROR: ssl_handshake ret(-0x%x)\r\n", -ret);
            ret = -1;
        }
    } while (0);

    if (cacert) {
        x509_crt_free(cacert);
        polarssl_free(cacert);
        cacert = NULL;
    }

    if (_cli_crt) {
        x509_crt_free(_cli_crt);
        polarssl_free(_cli_crt);
        _cli_crt = NULL;
    }

    if (_clikey_rsa) {
        pk_free(_clikey_rsa);
        polarssl_free(_clikey_rsa);
        _clikey_rsa = NULL;
    }

    if (ret < 0) {
        if (ssl_client->socket >= 0) {
            net_close(ssl_client->socket);
            ssl_client->socket = -1;
        }

        if (ssl_client->ssl != NULL) {
            ssl_free(ssl_client->ssl);
            free(ssl_client->ssl);
            ssl_client->ssl = NULL;
        }
    }

    return ssl_client->socket;
}

void stop_ssl_socket(sslclient_context *ssl_client)
{
    net_close(ssl_client->socket);
    ssl_client->socket = -1;

    if (ssl_client->ssl != NULL) {
        ssl_free(ssl_client->ssl);
        free(ssl_client->ssl);
    }
}

int send_ssl_data(sslclient_context *ssl_client, const uint8_t *data, uint16_t len)
{
    int ret = -1;

    if (ssl_client->ssl != NULL) {
        ret = ssl_write(ssl_client->ssl, data, len);
    }

    return ret;
}

int get_ssl_receive(sslclient_context *ssl_client, uint8_t* data, int length, int flag)
{
    int ret = 0;
    uint8_t has_backup_recvtimeout = 0;
    int backup_recv_timeout, recv_timeout;
    socklen_t len;

    if (ssl_client->ssl == NULL) {
        return 0;
    }

    if (flag & 0x01) {
        // peek for 10ms
        ret = lwip_getsockopt(ssl_client->socket, SOL_SOCKET, SO_RCVTIMEO, &backup_recv_timeout, &len);
        if (ret >= 0) {
            recv_timeout = 10;
            ret = lwip_setsockopt(ssl_client->socket, SOL_SOCKET, SO_RCVTIMEO, &recv_timeout, sizeof(recv_timeout));
            if (ret >= 0) {
                has_backup_recvtimeout = 1;
            }
        }
    }

    memset(data, 0, length);
    ret = ssl_read(ssl_client->ssl, data, length);

    if ((flag & 0x01) && (has_backup_recvtimeout == 1)) {
        // restore receiving timeout
        lwip_setsockopt(ssl_client->socket, SOL_SOCKET, SO_RCVTIMEO, &backup_recv_timeout, sizeof(recv_timeout));
    }

    return ret;
}

int get_ssl_sock_errno(sslclient_context *ssl_client) {
    int so_error;
    socklen_t len = sizeof(so_error);
    getsockopt(ssl_client->socket, SOL_SOCKET, SO_ERROR, &so_error, &len);
    return so_error;
}

int get_ssl_bytes_avail(sslclient_context *ssl_client) {
    if (ssl_client->ssl != NULL) {
        return ssl_get_bytes_avail(ssl_client->ssl);
    } else {
        return 0;
    }
}
