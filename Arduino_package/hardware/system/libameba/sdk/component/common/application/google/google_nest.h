#ifndef _GOOGLENEST_H_
#define _GOOGLENEST_H_

#include "platform_opts.h"

#define GOOGLENEST_TLS_POLARSSL       0    /*!< Use PolarSSL for TLS when GOOGLENEST */
#define GOOGLENEST_TLS_MBEDTLS        1    /*!< Use mbedTLS for TLS when GOOGLENEST */

#if CONFIG_USE_POLARSSL
#define GOOGLENEST_USE_TLS            GOOGLENEST_TLS_POLARSSL
#elif CONFIG_USE_MBEDTLS
#define GOOGLENEST_USE_TLS            GOOGLENEST_TLS_MBEDTLS
#endif

#define BUFFER_SIZE       512

typedef struct {
	int socket;
	void *tls;	
	char *host;
} googlenest_context;

int gn_connect(googlenest_context *googlenest, char *host, int port);
void gn_close(googlenest_context *googlenest);
int gn_put(googlenest_context *googlenest, char *uri, char *content);
int gn_patch(googlenest_context *googlenest, char *uri, char *content);
int gn_post(googlenest_context *googlenest, char *uri, char *content, unsigned char *out_buffer, unsigned int out_len);
int gn_get(googlenest_context *googlenest, char *uri, unsigned char *out_buffer, unsigned int out_len);
int gn_delete(googlenest_context *googlenest, char *uri);
int gn_stream(googlenest_context *googlenest, char *uri);
void google_retrieve_data_hook_callback(void (*callback)(char *));

void *gn_tls_connect(int *sock , char *host, int port);
void gn_tls_close(void *tls_in,int *sock);
int gn_tls_write(void *tls_in, char *request, int request_len);
int gn_tls_read(void *tls_in, char *buffer, int buf_len);
#endif