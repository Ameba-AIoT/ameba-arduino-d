/*
 * SSL/TLS interface functions for PolarSSL
 * Copyright (c) 2004-2009, Jouni Malinen <j@w1.fi>
 *
 * This software may be distributed under the terms of the BSD license.
 * See README for more details.
 */

#include "utils/includes.h"
#include "utils/common.h"
#include "tls.h"

#define DEBUG_LEVEL   0 // For debug: 5

extern int max_buf_bio_size;

int ErrorCnt = 0;

#define EAP_MBEDTLS_KEY_BLOCK_SIZE 256
#define EAP_MBEDTLS_MASTER_SECRET_SIZE 48
#define EAP_MBEDTLS_RANDOM_SIZE 32

struct tls_connection {
	// buffer BIO info (ptr, offset, len)
	struct buf_BIO *buf_in;
	struct buf_BIO *buf_out;

	/*
	key_block =
		client_write_MAC_secret[hash_size]
		server_write_MAC_secret[hash_size]
		client_write_key[Key_material_length]
		server_write_key[key_material_length]
		client_write_IV[IV_size]
		server_write_IV[IV_size]
		session_key_seed[40]
		ServerChallenge[16]
		ClientChallenge[16]
	*/
	unsigned char keyblk[EAP_MBEDTLS_KEY_BLOCK_SIZE];
	unsigned int keylen;
	size_t maclen;
	size_t ivlen;

	u8 master_secret[EAP_MBEDTLS_MASTER_SECRET_SIZE];
	u8 client_random[EAP_MBEDTLS_RANDOM_SIZE];
	u8 server_random[EAP_MBEDTLS_RANDOM_SIZE];

	int  (*tls_prf)(const unsigned char *, size_t, const char *,
                    const unsigned char *, size_t,
                    unsigned char *, size_t);

	//EAP-FAST
	tls_session_ticket_cb session_ticket_cb;
	void *session_ticket_cb_ctx;

};
// buffer BIO info (ptr, len, len left), might improve the performance if using circular buffer
struct buf_BIO{
	unsigned char *ptr;
	int len;
	int len_left;
};


/* used to test the buffer I/O read write functions
int test_BIO(struct tls_connection * conn){
	// test
	char * a = "abcdefg";
	printf("p: %d %d\n", conn->buf_out->len, conn->buf_out->len_left);
	buf_write(conn->buf_out, a, 8);
	printf("buf: %s\n", conn->buf_out->ptr);
	char b[5];
	char c[4];
	buf_read(conn->buf_out, b, 4);
	printf("buf: %s\n", conn->buf_out->ptr);
	b[4] = '\0';
	printf("%s\n", b);
	buf_read(conn->buf_out, c, 2);
	printf("buf: %s\n", conn->buf_out->ptr);
	c[3] = '\0';
	printf("%s\n", c);
	buf_write(conn->buf_out, a, 8);
	char d[10];
	buf_read(conn->buf_out, d, 10);
	d[1] = 'x';
	printf("%s\n", d);
}
*/

static volatile size_t min_heap_size = 0;


static int my_random(void *p_rng, unsigned char *output, size_t output_len)
{
	/* To avoid gcc warnings */
	( void ) p_rng;
	
	rtw_get_random_bytes(output, output_len);
	return 0;
}

void my_debug(void *ctx, int level, const char *str)
{
	/* To avoid gcc warnings */
	( void ) ctx;

	if(level <= DEBUG_LEVEL) {
		printf("\n\r%s", str);
	}
}

#if CONFIG_USE_POLARSSL

#include <polarssl/ssl.h>
#include <polarssl/memory.h>

void* eap_my_malloc(size_t size)
{
	void *ptr = pvPortMalloc(size);
	size_t current_heap_size = xPortGetFreeHeapSize();

	if((current_heap_size < min_heap_size) || (min_heap_size == 0))
		min_heap_size = current_heap_size;

	return ptr;
}

int buf_init(struct tls_connection * conn){
	conn->buf_in = (struct buf_BIO *)os_zalloc(sizeof(struct buf_BIO));
	if(conn->buf_in == NULL){
		return -1;
	}
	conn->buf_in->ptr = (unsigned char *)os_zalloc(max_buf_bio_size);
	if(conn->buf_in->ptr == NULL){
		return -1;
	}
	
	conn->buf_out = (struct buf_BIO *)os_zalloc(sizeof(struct buf_BIO));
	if(conn->buf_out == NULL){
		return -1;
	}
	conn->buf_out->ptr = (unsigned char *)os_zalloc(max_buf_bio_size);
	if(conn->buf_out->ptr == NULL){
		return -1;
	}
	
	conn->buf_in->len = 0;
	conn->buf_out->len = 0;
	conn->buf_in->len_left = max_buf_bio_size;
	conn->buf_out->len_left = max_buf_bio_size;
	return 1;
}

int buf_read(void *ctx, unsigned char *buf, size_t len){
	struct buf_BIO *bio = ctx;
	size_t read_len = len;
	wpa_printf(MSG_DEBUG, "TLS: buffer read size: %d", len);
	if(bio->len == 0){
		return 0;
	}
	if(bio->len < read_len)
		read_len = bio->len;
	os_memcpy(buf, bio->ptr, read_len);
	bio->len -= read_len;
	bio->len_left += read_len;
	os_memset(bio->ptr, 0, read_len);
	os_memmove(bio->ptr, bio->ptr + read_len, bio->len);
	//wpa_printf(MSG_INFO, "TLS: buffer read finish");
	return read_len;
}

int buf_write(void *ctx, const unsigned char * buf, size_t len){
	struct buf_BIO *bio = ctx;
	wpa_printf(MSG_DEBUG, "TLS: buffer write size: %d", len);
	if(bio->len_left < len){
		wpa_printf(MSG_INFO, "TLS: failed to write buffer due to size not enough, required size: %d", len);
		return -1;
	}
	os_memcpy(bio->ptr + bio->len, buf, len);
	bio->len += len;
	bio->len_left -= len;
	//wpa_printf(MSG_INFO, "TLS: buffer write finish");
	return len;
}

void buf_clear(void *ctx, int isIn){
	struct buf_BIO *bio = ctx;
	if(isIn == 1)
		wpa_printf(MSG_DEBUG, "TLS: clear input buffer, len: %d", bio->len);
	else
		wpa_printf(MSG_DEBUG, "TLS: clear output buffer, len: %d", bio->len);
	os_memset(bio->ptr, 0, max_buf_bio_size);
	bio->len = 0;
	bio->len_left = max_buf_bio_size;
}

void * tls_init(const struct tls_config *conf)
{
	ssl_context *ssl;
	int ret = -1;

	memory_set_own(eap_my_malloc, vPortFree);
	
	ssl = os_zalloc(sizeof(*ssl));
	if(ssl == NULL)
		return NULL;
	
	if((ret = ssl_init(ssl)) != 0){
		wpa_printf(MSG_INFO, "TLS: ssl_init() failed, ret: %d", ret);
		return NULL;
	}
	
	return ssl;
}

void tls_deinit(void *ssl_ctx)
{
	if(ssl_ctx != NULL){
		ssl_free(ssl_ctx);
		os_free(ssl_ctx, 0);
		ssl_ctx = NULL;
	}
}


int tls_get_errors(void *tls_ctx)
{
	wpa_printf(MSG_DEBUG, "TLS: tls_get_errors");
	return ErrorCnt;
}


struct tls_connection * tls_connection_init(void *tls_ctx)
{
	ssl_context *ssl = tls_ctx;
	struct tls_connection *conn;
	
	conn = os_zalloc(sizeof(*conn));
	if(conn == NULL)
		return NULL;

	conn->tls_prf = NULL;
	
	// init buf in conn
	if(buf_init(conn) < 0){
		wpa_printf(MSG_INFO, "TLS: buf_new() failed");
		tls_connection_deinit(tls_ctx, conn);
		return NULL;
	}
	
	//test_BIO(conn);
	
	ssl_set_endpoint(ssl, SSL_IS_CLIENT);
	ssl_set_authmode(ssl, SSL_VERIFY_NONE);
	ssl_set_rng(ssl, my_random, NULL);
	ssl_set_bio(ssl, buf_read, conn->buf_in, buf_write, conn->buf_out);
	ssl_set_dbg(ssl, my_debug, NULL);

#if defined(POLARSSL_DEBUG_C)
	debug_set_threshold(DEBUG_LEVEL);
#endif
	
	ErrorCnt = 0;
	
	return conn;
}


void tls_connection_deinit(void *tls_ctx, struct tls_connection *conn)
{
	wpa_printf(MSG_DEBUG, "TLS: tls_connection_deinit start");
	if(conn != NULL){
		os_free(conn->buf_in->ptr, 0);
		os_free(conn->buf_out->ptr, 0);
		os_free(conn->buf_in, 0);
		os_free(conn->buf_out, 0);
		os_free(conn, 0);
		conn = NULL;
	}
	ErrorCnt = 0;
}


int tls_connection_established(void *tls_ctx, struct tls_connection *conn)
{
	
	ssl_context *ssl = tls_ctx;
	if(ssl->state == SSL_HANDSHAKE_OVER){
		wpa_printf(MSG_DEBUG, "TLS: Check conn established.. true");
		return 1;
	}
	else{
		wpa_printf(MSG_DEBUG, "TLS: Check conn established.. false");
		return 0;
	}
}


int tls_connection_shutdown(void *tls_ctx, struct tls_connection *conn)
{
	if(conn == NULL)
		return -1;
	tls_connection_deinit(tls_ctx, conn);
	return -1;
}

int tls_connection_set_params(void *tls_ctx, struct tls_connection *conn,
			      const struct tls_connection_params *params)
{
	wpa_printf(MSG_DEBUG, "TLS: tls_connection_set_params");
	//return -1;
	return 0;
}


int tls_global_set_params(void *tls_ctx,
			  const struct tls_connection_params *params)
{
	wpa_printf(MSG_DEBUG, "TLS: tls_global_set_params");
	return -1;
}


int tls_global_set_verify(void *tls_ctx, int check_crl)
{
	wpa_printf(MSG_DEBUG, "TLS: tls_global_set_verify");
	return -1;
}


int tls_connection_set_verify(void *tls_ctx, struct tls_connection *conn,
			      int verify_peer, unsigned int flags,
			      const u8 *session_ctx, size_t session_ctx_len)
{
	wpa_printf(MSG_DEBUG, "TLS: tls_connection_set_verify");
	return -1;
}


int tls_connection_get_random(void *tls_ctx, struct tls_connection *conn,
			      struct tls_random *keys)
{
	ssl_context *ssl = tls_ctx;

	if(ssl == NULL || conn == NULL || keys == NULL)
		return -1;
	
	os_memset(keys, 0, sizeof(*keys));
	keys->client_random = conn->client_random;
	keys->client_random_len = 32;
	keys->server_random = conn->server_random;
	keys->server_random_len = 32;

	return 0;
}

// return 0: success
int tls_connection_prf(void *tls_ctx, struct tls_connection *conn,
		       const char *label, int server_random_first,
		       int skip_keyblock, u8 *out, size_t out_len)
{
	//wpa_printf(MSG_DEBUG, "TLS: tls_connection_prf");
	ssl_context *ssl = tls_ctx;
	ssl_session *session = ssl->session;

	int ret = 1;
	unsigned char *rnd;

	rnd = (unsigned char *)os_zalloc(64);
	if(!rnd){
		wpa_printf(MSG_INFO, "TLS: rnd buf alloc failed");
		return ret;
	}

	os_memcpy(rnd, conn->client_random, 32);
	os_memcpy(rnd + 32, conn->server_random, 32);
	
//	dump_buf(conn->client_random, 32);
//	dump_buf(conn->server_random, 32);
//	dump_buf(session->master, 48);

	if(conn->tls_prf != NULL)
		ret = conn->tls_prf( session->master, 48, label, rnd, 64, out, out_len );

	os_free(rnd, 0);

	return ret;
}

struct wpabuf * tls_connection_handshake(void *tls_ctx,
					 struct tls_connection *conn,
					 const struct wpabuf *in_data,
					 struct wpabuf **appl_data)
{
	ssl_context *ssl = tls_ctx;
	struct wpabuf * out_data;
	int size;
	int ret = 0;

	// clear input and output buffer
	buf_clear(conn->buf_out, 0);
	buf_clear(conn->buf_in, 1);

	// write input data to input buffer
	if(in_data && wpabuf_len(in_data) > 0 &&
		buf_write(conn->buf_in, wpabuf_head(in_data), wpabuf_len(in_data)) < 0){
		return NULL;
	}
	
	while( ssl->state != SSL_HANDSHAKE_OVER )
	{
		wpa_printf(MSG_INFO, "TLS: connection handshake, state: %d", ssl->state);
		//printf("\nTLS: connection handshake, state: %d\n", ssl->state);
		
		ret = ssl_handshake_step( ssl );

		// keep the client random & server random for eap further use
		if(ssl->state == SSL_CLIENT_CERTIFICATE){
			os_memcpy(conn->client_random, ssl->handshake->randbytes, 32);
			os_memcpy(conn->server_random, ssl->handshake->randbytes + 32, 32);
		}
		// free memory since server cert has been verified
		else if(ssl->state == SSL_SERVER_KEY_EXCHANGE){
			eap_server_cert_free();
		}
		// free memory since client cert has been set
		else if(ssl->state == SSL_CLIENT_CHANGE_CIPHER_SPEC){
			eap_client_cert_free();
		}
		// keep the tls_prf function pointer
		else if(ssl->state == SSL_SERVER_CHANGE_CIPHER_SPEC)
			conn->tls_prf = ssl->handshake->tls_prf;

		// time to send data out or read data in
		if(ret != 0){
			// need to read more data
			if(ret == POLARSSL_ERR_SSL_CONN_EOF)
				break;
			// handshake got error
			else{
				//wpa_printf(MSG_INFO, "TLS: connection handshake failed, ret: %d", ret);
				printf("\nTLS: connection handshake failed, ret: %d\n", ret);
				ErrorCnt = 1;
				return NULL;
			}
		}
	}

	// store output buffer to out_data
	size = conn->buf_out->len;
	out_data = wpabuf_alloc(size);
	if(out_data == NULL){
		wpa_printf(MSG_INFO, "SSL: Failed to allocate memory for "
			   "handshake output (%d bytes)", size);
		return NULL;
	}
	buf_read(conn->buf_out, out_data->buf, size);
	wpabuf_put(out_data, size);

	//dump_buf(out_data->buf, size);

	// clear input and output buffer
	buf_clear(conn->buf_out, 0);
	buf_clear(conn->buf_in, 1);
	
	return out_data;
	
}


struct wpabuf * tls_connection_server_handshake(void *tls_ctx,
						struct tls_connection *conn,
						const struct wpabuf *in_data,
						struct wpabuf **appl_data)
{
	wpa_printf(MSG_DEBUG, "TLS: tls_connection_server_handshake");
	return NULL;
}


struct wpabuf * tls_connection_encrypt(void *tls_ctx,
				       struct tls_connection *conn,
				       const struct wpabuf *in_data)
{
	wpa_printf(MSG_DEBUG, "TLS: tls_connection_encrypt");

	ssl_context *ssl = tls_ctx;
	struct wpabuf * out_data;
	int size;
	int res;

	if(conn == NULL)
		return NULL;

	res = ssl_write(ssl, wpabuf_head(in_data), wpabuf_len(in_data));
	if(res < 0){
		wpa_printf(MSG_INFO, "TLS: tls_connection_encrypt failed - ssl_write");
		return NULL;
	}

	/* Read encrypted data to be sent to the server */
	size = wpabuf_len(in_data) + 300;
	out_data = wpabuf_alloc(size);
	if(out_data == NULL){
		wpa_printf(MSG_INFO, "TLS: Failed to allocate memory for "
			   "encrypted output (%d bytes)", size);
		return NULL;
	}

	res = buf_read(conn->buf_out, out_data->buf, size);
	if(res < 0){
		wpa_printf(MSG_INFO, "TLS: tls_connection_decrypt failed - buf_read");
		wpabuf_free(out_data);
		return NULL;
	}
	wpabuf_put(out_data, res);

	// clear input and output buffer
	buf_clear(conn->buf_out, 0);
	buf_clear(conn->buf_in, 1);
	
	return out_data;
}


struct wpabuf * tls_connection_decrypt(void *tls_ctx,
				       struct tls_connection *conn,
				       const struct wpabuf *in_data)
{
	wpa_printf(MSG_DEBUG, "TLS: tls_connection_decrypt");
	
	ssl_context *ssl = tls_ctx;
	struct wpabuf * out_data;
	int size;
	int res;

	// write input data to input buffer
	if(in_data && wpabuf_len(in_data) > 0 &&
		buf_write(conn->buf_in, wpabuf_head(in_data), wpabuf_len(in_data)) < 0){
		wpa_printf(MSG_INFO, "TLS: tls_connection_decrypt failed - buf_write");
		return NULL;
	}

	/* Read decrypted data for further processing */
	/*
	 * refer to tls_openssl.c
	 *
	 * Even though we try to disable TLS compression, it is possible that
	 * this cannot be done with all TLS libraries. Add extra buffer space
	 * to handle the possibility of the decrypted data being longer than
	 * input data.
	 */
	size = (wpabuf_len(in_data) + 500) * 3;
	out_data = wpabuf_alloc(size);
	if(out_data == NULL){
		wpa_printf(MSG_INFO, "TLS: Failed to allocate memory for "
			   "decrypted output (%d bytes)", size);
		return NULL;
	}
	
	res = ssl_read(ssl, out_data->buf, size);
	if(res < 0){
		wpa_printf(MSG_INFO, "TLS: tls_connection_decrypt failed - ssl_read");
		wpabuf_free(out_data);
		return NULL;
	}
	wpabuf_put(out_data, res);

	//dump_buf(out_data->buf, size);

	// clear input and output buffer
	buf_clear(conn->buf_out, 0);
	buf_clear(conn->buf_in, 1);
	
	return out_data;
}


int tls_connection_client_hello_ext(void *tls_ctx, struct tls_connection *conn,
				    int ext_type, const u8 *data,
				    size_t data_len)
{
	/* To avoid gcc warnings */
	( void ) conn;
	( void ) tls_ctx;
	( void ) ext_type;
	( void ) data;
	( void ) data_len;

	wpa_printf(MSG_DEBUG, "TLS: tls_connection_client_hello_ext");
	return -1;
}

int tls_connection_set_cipher_list(void *tls_ctx, struct tls_connection *conn,
				   u8 *ciphers)
{
	/* To avoid gcc warnings */
	( void ) conn;
	( void ) tls_ctx;
	( void ) ciphers;

	wpa_printf(MSG_DEBUG, "TLS: tls_connection_set_cipher_list");
	return -1;
}


int tls_get_version(void *ssl_ctx, struct tls_connection *conn,
		    char *buf, size_t buflen)
{
	/* To avoid gcc warnings */
	( void ) conn;
	( void ) ssl_ctx;
	( void ) buf;
	( void ) buflen;

	wpa_printf(MSG_DEBUG, "TLS: tls_get_version");
	return -1;
}


int tls_get_cipher(void *tls_ctx, struct tls_connection *conn,
		   char *buf, size_t buflen)
{
	/* To avoid gcc warnings */
	( void ) conn;
	( void ) tls_ctx;
	( void ) buf;
	( void ) buflen;

	wpa_printf(MSG_DEBUG, "TLS: tls_get_cipher");
	return -1;
}
#elif CONFIG_USE_MBEDTLS /* CONFIG_USE_POLARSSL */
#if defined(MBEDTLS_VERSION_NUMBER) && (MBEDTLS_VERSION_NUMBER>=0x03010000)
#include <mbedtls/net_sockets.h>
#include "ssl_misc.h"
#else
#include <mbedtls/ssl.h>
#include <mbedtls/net.h>
#include <mbedtls/ssl_internal.h>
#endif
#include <mbedtls/debug.h>

struct buf_BIO *conn_buf_out, *conn_buf_in;

#define MBED_TLS_DEBUG_ENABLE 1

#if defined(MBED_TLS_DEBUG_ENABLE) && (MBED_TLS_DEBUG_ENABLE == 1)
static void mbedtls_debug(void *ctx, int level, const char *file, int line, const char *str)
{
	/* To avoid gcc warnings */
	( void ) ctx;

	int filename_idx = 0;
	int cnt = 0;
	
	//get filename only
	while(*(file+cnt) != '\0'){
		if(*(file+cnt) == '/'){
			filename_idx = cnt+1;
		}
		cnt++;
	}

	if(level <= DEBUG_LEVEL) {
		printf("\n\r%s:%d: %s\n\r", file+filename_idx, line, str);
	}
}
#endif

int buf_init(struct tls_connection * conn){
	conn->buf_in = (struct buf_BIO *)os_zalloc(sizeof(struct buf_BIO));
	if(conn->buf_in == NULL){
		return -1;
	}
	conn->buf_in->ptr = (unsigned char *)os_zalloc(max_buf_bio_size);
	if(conn->buf_in->ptr == NULL){
		return -1;
	}
	
	conn->buf_out = (struct buf_BIO *)os_zalloc(sizeof(struct buf_BIO));
	if(conn->buf_out == NULL){
		return -1;
	}
	conn->buf_out->ptr = (unsigned char *)os_zalloc(max_buf_bio_size);
	if(conn->buf_out->ptr == NULL){
		return -1;
	}
	
	conn->buf_in->len = 0;
	conn->buf_out->len = 0;
	conn->buf_in->len_left = max_buf_bio_size;
	conn->buf_out->len_left = max_buf_bio_size;

	conn_buf_out = conn->buf_out;
	conn_buf_in = conn->buf_in;
	return 1;
}

int buf_read_store(void *ctx, unsigned char *buf, size_t len){
	struct buf_BIO *bio = ctx;

	size_t read_len = len;
	wpa_printf(MSG_DEBUG, "TLS: buffer read size: %d", len);
	if(bio->len == 0){
		return 0;
	}
	if((size_t)bio->len < read_len)
		read_len = bio->len;
	os_memcpy(buf, bio->ptr, read_len);
	bio->len -= read_len;
	bio->len_left += read_len;
	os_memset(bio->ptr, 0, read_len);
	os_memmove(bio->ptr, bio->ptr + read_len, bio->len);
	//wpa_printf(MSG_INFO, "TLS: buffer read finish");
	return read_len;
}

int buf_write_store(void *ctx, const unsigned char * buf, size_t len){
	struct buf_BIO *bio = ctx;

	wpa_printf(MSG_DEBUG, "TLS: buffer write size: %d", len);
	if((size_t)bio->len_left < len){
		wpa_printf(MSG_INFO, "TLS: failed to write buffer due to size not enough, required size: %d", len);
		return -1;
	}
	os_memcpy(bio->ptr + bio->len, buf, len);
	bio->len += len;
	bio->len_left -= len;
	//wpa_printf(MSG_INFO, "TLS: buffer write finish");
	return len;
}

int buf_read(void *ctx, unsigned char *buf, size_t len)
{
	/* To avoid gcc warnins */
	( void ) ctx;
	
	struct buf_BIO *bio = conn_buf_in; 

	size_t read_len = len;
	wpa_printf(MSG_DEBUG, "TLS: buffer read size: %d", len);
	if(bio->len == 0){
		return 0;
	}
	if((size_t)bio->len < read_len)
		read_len = bio->len;
	os_memcpy(buf, bio->ptr, read_len);
	bio->len -= read_len;
	bio->len_left += read_len;
	os_memset(bio->ptr, 0, read_len);
	os_memmove(bio->ptr, bio->ptr + read_len, bio->len);
	//wpa_printf(MSG_INFO, "TLS: buffer read finish");
	return read_len;
}



int buf_write(void *ctx, const unsigned char * buf, size_t len)
{
	/* To avoid gcc warnins */
	( void ) ctx;

	struct buf_BIO *bio = conn_buf_out;
             
	wpa_printf(MSG_DEBUG, "TLS: buffer write size: %d", len);
	if((size_t)bio->len_left < len){
		wpa_printf(MSG_INFO, "TLS: failed to write buffer due to size not enough, required size: %d", len);
		return -1;
	}
	os_memcpy(bio->ptr + bio->len, buf, len);
	bio->len += len;
	bio->len_left -= len;
	//wpa_printf(MSG_INFO, "TLS: buffer write finish");
	return len;
}

void buf_clear(void *ctx, int isIn){
	struct buf_BIO *bio = ctx;
	if(isIn == 1)
		wpa_printf(MSG_DEBUG, "TLS: clear input buffer, len: %d", bio->len);
	else
		wpa_printf(MSG_DEBUG, "TLS: clear output buffer, len: %d", bio->len);
	os_memset(bio->ptr, 0, max_buf_bio_size);
	bio->len = 0;
	bio->len_left = max_buf_bio_size;
}

static void* my_calloc(size_t nelements, size_t elementSize)
{
	size_t size;
	void *ptr = NULL;

	size = nelements * elementSize;
	ptr = pvPortMalloc(size);

	if(ptr)
		memset(ptr, 0, size);

	return ptr;
}

struct eap_tls{
	void *ssl;
	void *conf;
	void *fd;
};

extern int mbedtls_platform_set_calloc_free( void * (*calloc_func)( size_t, size_t ),void (*free_func)( void * ) );
void * tls_init(const struct tls_config *conf)
{
	/* To avoid gcc warnings */
	( void ) conf;
	
	struct eap_tls *tls_context;

	mbedtls_platform_set_calloc_free(my_calloc, vPortFree);

	tls_context = os_zalloc(sizeof(struct eap_tls));

	tls_context->ssl = os_zalloc(sizeof(mbedtls_ssl_context));
	tls_context->conf = os_zalloc(sizeof(mbedtls_ssl_config));
	tls_context->fd = os_zalloc(sizeof(mbedtls_net_context));

	if((tls_context == NULL)||(tls_context->ssl == NULL)||(tls_context->conf == NULL)||(tls_context->fd == NULL))
		return NULL;

	mbedtls_net_init(tls_context->fd);
	mbedtls_ssl_init(tls_context->ssl);
	mbedtls_ssl_config_init(tls_context->conf);

	return tls_context;
}

void tls_deinit(void *ssl_ctx)
{
	if(ssl_ctx != NULL){
		struct eap_tls *tls_context = (struct eap_tls *) ssl_ctx;
		mbedtls_net_free(tls_context->fd);
		os_free(tls_context->fd, 0);
		mbedtls_ssl_free(tls_context->ssl);
		os_free(tls_context->ssl, 0);
		mbedtls_ssl_config_free(tls_context->conf);
		os_free(tls_context->conf, 0);
		os_free(ssl_ctx, 0);
		ssl_ctx = NULL;
	}
}

int tls_get_errors(void *tls_ctx)
{
	/* To avoid gcc warnings */
	( void ) tls_ctx;

	wpa_printf(MSG_DEBUG, "TLS: tls_get_errors");
	return ErrorCnt;
}

#if defined(MBEDTLS_VERSION_NUMBER) && (MBEDTLS_VERSION_NUMBER>=0x03010000)
	static void tls_connection_key_derivation( void *p_expkey,
											mbedtls_ssl_key_export_type type,
											const unsigned char *secret,
											size_t secret_len,
											const unsigned char client_random[32],
											const unsigned char server_random[32],
											mbedtls_tls_prf_types tls_prf_type ){

		/* To avoid gcc warnings */
		( void ) type;
		( void ) secret_len;
		( void ) tls_prf_type;

		struct tls_connection *conn = (struct tls_connection *)p_expkey;

		memcpy( conn->master_secret, secret, sizeof( conn->master_secret ) );
		memcpy( conn->client_random, client_random, sizeof( conn->client_random ) );
		memcpy( conn->server_random, server_random, sizeof( conn->server_random ) );

	}
#else
#if defined(MBEDTLS_SSL_EXPORT_KEYS)
static int tls_connection_key_derivation( void *p_expkey,
                                const unsigned char *ms,
                                const unsigned char *kb,
                                size_t maclen,
                                size_t keylen,
                                size_t ivlen ){

	struct tls_connection *conn = (struct tls_connection *)p_expkey;

	conn->keylen = keylen;
	conn->ivlen = ivlen;
	conn->maclen= maclen;

	memcpy( conn->keyblk, kb, sizeof( conn->keyblk ) );
	memcpy( conn->master_secret, ms, sizeof( conn->master_secret ) );

	/*
	if(ms){
		printf("ms: ");
		for(int i=0; i<sizeof( conn->master_secret ); i++){
			 printf("%02x ", conn->master_secret[i]);
		}
		printf("\r\n");
	}

	if(kb){
		printf("kb: ");
		for(int i=0; i<sizeof( conn->keyblk ); i++){
	    	 printf("%02x ", conn->keyblk[i]);
		}
		printf("\r\n");
	}

	if(maclen) printf("maclen: %d\r\n", conn->maclen);
	if(ivlen) printf("ivlen: %d\r\n", conn->ivlen);
	if(keylen) printf("keylen: %d\r\n", conn->keylen);
	*/

	return( 0 );

}
#endif
#endif

struct tls_connection * tls_connection_init(void *tls_ctx)
{
	volatile int ret;
	struct eap_tls *tls_context = (struct eap_tls *) tls_ctx;

	struct tls_connection *conn;
	conn = os_zalloc(sizeof(*conn));
	if(conn == NULL)
		return NULL;

	conn->tls_prf = NULL;
	
	// init buf in conn
	if(buf_init(conn) < 0){
		wpa_printf(MSG_INFO, "TLS: buf_new() failed");
		tls_connection_deinit(tls_ctx, conn);
		return NULL;
	}

	//test_BIO(conn);
	mbedtls_ssl_set_bio(tls_context->ssl, tls_context->fd, buf_write, buf_read, NULL);

	if((ret = mbedtls_ssl_config_defaults(tls_context->conf,
			MBEDTLS_SSL_IS_CLIENT,
			MBEDTLS_SSL_TRANSPORT_STREAM,
			MBEDTLS_SSL_PRESET_DEFAULT)) != 0) {

		wpa_printf(MSG_INFO, "TLS: mbedtls_ssl_config_defaults() failed");
		return NULL;
	}

	mbedtls_ssl_conf_authmode(tls_context->conf, MBEDTLS_SSL_VERIFY_NONE);
	mbedtls_ssl_conf_rng(tls_context->conf, my_random, NULL);

#if defined(MBED_TLS_DEBUG_ENABLE) && (MBED_TLS_DEBUG_ENABLE == 1)
	mbedtls_ssl_conf_dbg(tls_context->conf, mbedtls_debug, NULL);
	// mbedtls_ssl_conf_min_version(tls_context->conf, MBEDTLS_SSL_MINOR_VERSION_3, MBEDTLS_SSL_MINOR_VERSION_3);
#endif

	if((ret = mbedtls_ssl_setup(tls_context->ssl, tls_context->conf)) != 0) {
		wpa_printf(MSG_INFO, "TLS: mbedtls_ssl_setup() failed");
		return NULL;
	}

#if defined(MBEDTLS_DEBUG_C)
	mbedtls_debug_set_threshold(DEBUG_LEVEL);
#endif


#if defined(MBEDTLS_VERSION_NUMBER) && (MBEDTLS_VERSION_NUMBER>=0x03010000)
	mbedtls_ssl_set_export_keys_cb((mbedtls_ssl_context *)tls_context->ssl, tls_connection_key_derivation, (void *)conn);
#else
#if defined(MBEDTLS_SSL_EXPORT_KEYS)
	mbedtls_ssl_conf_export_keys_cb((mbedtls_ssl_config *)tls_context->conf, tls_connection_key_derivation, (void *)conn);
#else
	wpa_printf(MSG_ERROR, "TLS: MBEDTLS_SSL_EXPORT_KEYS should be defined");
	return NULL;
#endif
#endif

	ErrorCnt = 0;

	return conn;
}


void tls_connection_deinit(void *tls_ctx, struct tls_connection *conn)
{
	/* To avoid gcc warnings */
	( void ) tls_ctx;
	
	wpa_printf(MSG_DEBUG, "TLS: tls_connection_deinit start");
	if(conn != NULL){
		os_free(conn->buf_in->ptr, 0);
		os_free(conn->buf_out->ptr, 0);
		os_free(conn->buf_in, 0);
		os_free(conn->buf_out, 0);
		os_free(conn, 0);
		conn = NULL;
	}
	ErrorCnt = 0;
}


int tls_connection_established(void *tls_ctx, struct tls_connection *conn)
{
	/* To avoid gcc warnings */
	( void ) conn;
	
	mbedtls_ssl_context *ssl = ((struct eap_tls *)tls_ctx)->ssl;

	if(ssl->state == MBEDTLS_SSL_HANDSHAKE_OVER){
		wpa_printf(MSG_DEBUG, "TLS: Check conn established.. true");
		return 1;
	}
	else{
		wpa_printf(MSG_DEBUG, "TLS: Check conn established.. false");
		return 0;
	}
}


int tls_connection_shutdown(void *tls_ctx, struct tls_connection *conn)
{
	if(conn == NULL)
		return -1;
	tls_connection_deinit(tls_ctx, conn);
	return -1;
}

int tls_connection_set_params(void *tls_ctx, struct tls_connection *conn,
				const struct tls_connection_params *params)
{
	/* To avoid gcc warnings */
	( void ) tls_ctx;
	( void ) conn;
	( void ) params;
	
	wpa_printf(MSG_DEBUG, "TLS: tls_connection_set_params");
	//return -1;
	return 0;
}


int tls_global_set_params(void *tls_ctx,
			  const struct tls_connection_params *params)
{
	/* To avoid gcc warnings */
	( void ) params;
	( void ) tls_ctx;
	
	wpa_printf(MSG_DEBUG, "TLS: tls_global_set_params");
	return -1;
}


int tls_global_set_verify(void *tls_ctx, int check_crl)
{
	/* To avoid gcc warnings */
	( void ) tls_ctx;
	( void ) check_crl;
	
	wpa_printf(MSG_DEBUG, "TLS: tls_global_set_verify");
	return -1;
}


int tls_connection_set_verify(void *tls_ctx, struct tls_connection *conn,
				int verify_peer, unsigned int flags,
				const u8 *session_ctx, size_t session_ctx_len)
{
	/* To avoid gcc warnings */
	( void ) tls_ctx;
	( void ) conn;
	( void ) verify_peer;
	( void ) flags;
	( void ) session_ctx;
	( void ) session_ctx_len;
	
	wpa_printf(MSG_DEBUG, "TLS: tls_connection_set_verify");
	return -1;
}


int tls_connection_get_random(void *tls_ctx, struct tls_connection *conn,
				struct tls_random *keys)
{
	mbedtls_ssl_context *ssl = ((struct eap_tls *)tls_ctx)->ssl;

	if(ssl == NULL || conn == NULL || keys == NULL)
		return -1;

	os_memset(keys, 0, sizeof(*keys));
	keys->client_random = conn->client_random;
	keys->client_random_len = 32;
	keys->server_random = conn->server_random;
	keys->server_random_len = 32;

	return 0;
}

// return 0: success
int tls_connection_prf(void *tls_ctx, struct tls_connection *conn,
			const char *label, int server_random_first,
			int skip_keyblock, u8 *out, size_t out_len)
{	
	/* To avoid gcc warnings */
	( void ) server_random_first;
	( void ) skip_keyblock;
	
	//wpa_printf(MSG_DEBUG, "TLS: tls_connection_prf");
	mbedtls_ssl_context *ssl = (( struct eap_tls *)tls_ctx)->ssl;
	mbedtls_ssl_session *session = ssl->session;

	int ret = 1;
	unsigned char *rnd;

	rnd = (unsigned char *)os_zalloc(64);
	if(!rnd){
		wpa_printf(MSG_INFO, "TLS: rnd buf alloc failed");
		return ret;
	}

	os_memcpy(rnd, conn->client_random, 32);
	os_memcpy(rnd + 32, conn->server_random, 32);
	
//	dump_buf(conn->client_random, 32);
//	dump_buf(conn->server_random, 32);
//	dump_buf(session->master, 48);

	if(conn->tls_prf != NULL)
		ret = conn->tls_prf( session->master, 48, label, rnd, 64, out, out_len );

	os_free(rnd, 0);

	return ret;
}

int tls_connection_get_eap_fast_key(void *tls_ctx, struct tls_connection *conn,
				    u8 *out, size_t out_len)
{


	mbedtls_ssl_context *ssl = ((struct eap_tls *)tls_ctx)->ssl;
	u8 *rnd;
	int sks_offset = 0;
	u8 *tmp_out = NULL;
	u8 *_out = out;
	u8 client_random[EAP_MBEDTLS_RANDOM_SIZE];
	u8 server_random[EAP_MBEDTLS_RANDOM_SIZE];
	unsigned char master_key[64];
	size_t master_key_len;
	int ret;

	/*
	 * TLS library did not support EAP-FAST key generation, so get the
	 * needed TLS session parameters and use an internal implementation of
	 * TLS PRF to derive the key.
	 */

	if (conn == NULL || ssl == NULL)
		return -1;

	// session_key_seed offset of key_blk
#if defined(MBEDTLS_VERSION_NUMBER) && (MBEDTLS_VERSION_NUMBER>=0x03010000)
	mbedtls_ssl_transform *transform = ssl->transform;
	int ciphersuite = ssl->session->ciphersuite;
	const mbedtls_ssl_ciphersuite_t *ciphersuite_info;
	int mac_key_len, enc_key_len, iv_copy_len;
	size_t keylen;
	mac_key_len =transform->maclen;
	ciphersuite_info = mbedtls_ssl_ciphersuite_from_id(ciphersuite);
#if defined(MBEDTLS_USE_PSA_CRYPTO)
	 psa_algorithm_t alg;
	 psa_key_type_t key_type;
	 size_t key_bits;
    if ((status = mbedtls_ssl_cipher_to_psa(ciphersuite_info->cipher,
                                            transform->taglen,
                                            &alg,
                                            &key_type,
                                            &key_bits)) != PSA_SUCCESS) {
    	return -1;
    }
    keylen = PSA_BITS_TO_BYTES(key_bits);
#else
    const mbedtls_cipher_info_t *cipher_info;
    cipher_info = mbedtls_cipher_info_from_type((mbedtls_cipher_type_t)ciphersuite_info->cipher);
    if (cipher_info == NULL) {
    	return -1;
    }
    keylen = mbedtls_cipher_info_get_key_bitlen(cipher_info) / 8;
#endif /* MBEDTLS_USE_PSA_CRYPTO */

	enc_key_len=keylen;
	iv_copy_len = (transform->fixed_ivlen) ?
	                      transform->fixed_ivlen : transform->ivlen;
	sks_offset = 2 * (enc_key_len + mac_key_len + iv_copy_len);
#else
	sks_offset = 2 * (conn->keylen + conn->maclen + conn->ivlen);
#endif
	if (sks_offset < 0)
		return -1;
	tmp_out = os_malloc(sks_offset + out_len);
	if (!tmp_out)
		return -1;
	_out = tmp_out;

	rnd = os_malloc(2 * EAP_MBEDTLS_RANDOM_SIZE);
	if (!rnd) {
		os_free(tmp_out, 0);
		return -1;
	}

	memcpy(client_random, conn->client_random, EAP_MBEDTLS_RANDOM_SIZE);
	memcpy(server_random, conn->server_random, EAP_MBEDTLS_RANDOM_SIZE);
	memcpy(master_key, conn->master_secret, EAP_MBEDTLS_MASTER_SECRET_SIZE);
	master_key_len = EAP_MBEDTLS_MASTER_SECRET_SIZE;

	os_memcpy(rnd, server_random, 32);
	os_memcpy(rnd + 32, client_random, 32);

	if(conn->tls_prf(master_key, master_key_len, "key expansion", rnd, 2 * 32,
				    _out, sks_offset + out_len) == 0){
		ret = 0;
	}

	os_free(rnd, 0);
	if (ret == 0)
		os_memcpy(out, _out + sks_offset, out_len);
	bin_clear_free(tmp_out, sks_offset);

	return 0;
}

static struct wpabuf * tls_get_appl_data(mbedtls_ssl_context *ssl, struct tls_connection *conn, size_t max_len)
{
	/* To avoid gcc warnings */
	( void ) conn;
	
	struct wpabuf *appl_data;
	int res;

	appl_data = wpabuf_alloc(max_len + 100);
	if (appl_data == NULL)
		return NULL;

	res = mbedtls_ssl_read(ssl, wpabuf_mhead(appl_data), wpabuf_size(appl_data));
	if(res < 0){
		wpa_printf(MSG_INFO, "TLS: tls_connection_decrypt failed - ssl_read");
		wpabuf_free(appl_data);
		return NULL;
	}
	wpabuf_put(appl_data, res);
	wpa_hexdump_buf_key(MSG_MSGDUMP, "SSL: Application Data in Finished "
			    "message", appl_data);

	return appl_data;
}


extern void eap_server_cert_free(void);
extern void eap_client_cert_free(void);
struct wpabuf * tls_connection_handshake(void *tls_ctx,
					 struct tls_connection *conn,
					 const struct wpabuf *in_data,
					 struct wpabuf **appl_data)
{
	if (appl_data)
		*appl_data = NULL;

	mbedtls_ssl_context *ssl = ((struct eap_tls*)tls_ctx)->ssl;

	struct wpabuf * out_data;
	int size;
	int ret = 0;

	// clear input and output buffer
	buf_clear(conn->buf_out, 0);
	buf_clear(conn->buf_in, 1);

	// write input data to input buffer
	if(in_data && wpabuf_len(in_data) > 0 &&
		buf_write_store(conn_buf_in, wpabuf_head(in_data), wpabuf_len(in_data)) < 0){
		return NULL;
	}

	while( ssl->state != MBEDTLS_SSL_HANDSHAKE_OVER )
	{
		wpa_printf(MSG_INFO, "TLS: connection handshake, state: %d", ssl->state);
		//printf("\nTLS: connection handshake, state: %d\n", ssl->state);
#if defined(MBEDTLS_PSA_CRYPTO_C) && defined(MBEDTLS_VERSION_NUMBER) && (MBEDTLS_VERSION_NUMBER>=0x03040000)
		psa_crypto_init();
#endif
		ret = mbedtls_ssl_handshake_step( ssl );

		// keep the client random & server random for eap further use
		if(ssl->state == MBEDTLS_SSL_CLIENT_CERTIFICATE){
			os_memcpy(conn->client_random, ssl->handshake->randbytes, 32);
			os_memcpy(conn->server_random, ssl->handshake->randbytes + 32, 32);
		}
		// free memory since server cert has been verified
		else if(ssl->state == MBEDTLS_SSL_SERVER_KEY_EXCHANGE){
			eap_server_cert_free();
		}
		// free memory since client cert has been set
		else if(ssl->state == MBEDTLS_SSL_CLIENT_CHANGE_CIPHER_SPEC){
			eap_client_cert_free();
		}
		// keep the tls_prf function pointer
		else if(ssl->state == MBEDTLS_SSL_SERVER_CHANGE_CIPHER_SPEC){
			conn->tls_prf = ssl->handshake->tls_prf;
		}
		//handle piggyback phase2 data
		else if (ssl->state == MBEDTLS_SSL_HANDSHAKE_OVER && appl_data && conn->buf_in->len > 0){
			*appl_data = tls_get_appl_data(ssl, conn, wpabuf_len(in_data));
		}

		// time to send data out or read data in
		if(ret != 0){
			// need to read more data
			if(ret == MBEDTLS_ERR_SSL_CONN_EOF)
				break;
			// handshake got error
			else{
				//wpa_printf(MSG_INFO, "TLS: connection handshake failed, ret: %d", ret);
				printf("\nTLS: connection handshake failed, ret: %d\n", ret);
				ErrorCnt = 1;
				return NULL;
			}
		}
	}

	// store output buffer to out_data
	size = conn_buf_out->len;
	out_data = wpabuf_alloc(size);
	if(out_data == NULL){
		wpa_printf(MSG_INFO, "SSL: Failed to allocate memory for "
			   "handshake output (%d bytes)", size);
		return NULL;
	}
	buf_read_store(conn_buf_out, out_data->buf, size);
	wpabuf_put(out_data, size);

	//dump_buf(out_data->buf, size);

	// clear input and output buffer
	buf_clear(conn->buf_out, 0);
	buf_clear(conn->buf_in, 1);

	return out_data;

}


struct wpabuf * tls_connection_server_handshake(void *tls_ctx,
						struct tls_connection *conn,
						const struct wpabuf *in_data,
						struct wpabuf **appl_data)
{
	
	/* To avoid gcc warnings */
	( void ) conn;
	( void ) tls_ctx;
	( void ) in_data;
	( void ) appl_data;
	
	wpa_printf(MSG_DEBUG, "TLS: tls_connection_server_handshake");
	return NULL;
}


struct wpabuf * tls_connection_encrypt(void *tls_ctx,
						struct tls_connection *conn,
						const struct wpabuf *in_data)
{
	wpa_printf(MSG_DEBUG, "TLS: tls_connection_encrypt");

	mbedtls_ssl_context *ssl = ((struct eap_tls *) tls_ctx)->ssl;
	struct wpabuf * out_data;
	int size;
	int res;

	if(conn == NULL)
		return NULL;

	res = mbedtls_ssl_write(ssl, wpabuf_head(in_data), wpabuf_len(in_data));
	if(res < 0){
		wpa_printf(MSG_INFO, "TLS: tls_connection_encrypt failed - ssl_write");
		return NULL;
	}

	/* Read encrypted data to be sent to the server */
	size = wpabuf_len(in_data) + 300;
	out_data = wpabuf_alloc(size);
	if(out_data == NULL){
		wpa_printf(MSG_INFO, "TLS: Failed to allocate memory for "
				"encrypted output (%d bytes)", size);
		return NULL;
	}

	res = buf_read_store(conn_buf_out, out_data->buf, size);
	if(res < 0){
		wpa_printf(MSG_INFO, "TLS: tls_connection_decrypt failed - buf_read");
		wpabuf_free(out_data);
		return NULL;
	}
	wpabuf_put(out_data, res);

	// clear input and output buffer
	buf_clear(conn->buf_out, 0);
	buf_clear(conn->buf_in, 1);

	return out_data;
}


struct wpabuf * tls_connection_decrypt(void *tls_ctx,
						struct tls_connection *conn,
						const struct wpabuf *in_data)
{
	wpa_printf(MSG_DEBUG, "TLS: tls_connection_decrypt");

	mbedtls_ssl_context *ssl = ((struct eap_tls *)tls_ctx)->ssl;
	struct wpabuf * out_data;
	int size;
	int res;

	// write input data to input buffer
	if(in_data && wpabuf_len(in_data) > 0 &&
		buf_write_store(conn_buf_in, wpabuf_head(in_data), wpabuf_len(in_data)) < 0){
				wpa_printf(MSG_INFO, "TLS: tls_connection_decrypt failed - buf_write");
		return NULL;
	}

	/* Read decrypted data for further processing */
	/*
	 * refer to tls_openssl.c
	 *
	 * Even though we try to disable TLS compression, it is possible that
	 * this cannot be done with all TLS libraries. Add extra buffer space
	 * to handle the possibility of the decrypted data being longer than
	 * input data.
	 */
	size = (wpabuf_len(in_data) + 500) * 3;
	out_data = wpabuf_alloc(size);
	if(out_data == NULL){
		wpa_printf(MSG_INFO, "TLS: Failed to allocate memory for "
			   "decrypted output (%d bytes)", size);
		return NULL;
	}

	res = mbedtls_ssl_read(ssl, out_data->buf, size);
	if(res < 0){
		wpa_printf(MSG_INFO, "TLS: tls_connection_decrypt failed - ssl_read");
		wpabuf_free(out_data);
		return NULL;
	}
	wpabuf_put(out_data, res);

	//dump_buf(out_data->buf, size);

	// clear input and output buffer
	buf_clear(conn->buf_out, 0);
	buf_clear(conn->buf_in, 1);

	return out_data;
}

int tls_connection_set_session_ticket_cb(void *tls_ctx,
					 struct tls_connection *conn,
					 tls_session_ticket_cb cb,
					 void *ctx)
{
	/* To avoid gcc warnings */
	( void ) tls_ctx;
	
	conn->session_ticket_cb = cb;
	conn->session_ticket_cb_ctx = ctx;

	//TODO to enable pac session resume, we need to add a callback function in parse hello to derive master
	//key with PAC key rfc4851#section-5.1

	return 0;
}


static int tls_set_session_ticket_ext(mbedtls_ssl_context *ssl, void *ext_data, int ext_len)
{
	wpa_printf(MSG_DEBUG, "TLS: tls_set_session_ticket_ext");

#if defined(MBEDTLS_VERSION_NUMBER) && (MBEDTLS_VERSION_NUMBER>=0x03010000)
#else
	if (ssl->conf->min_minor_ver >= MBEDTLS_SSL_MINOR_VERSION_1)
	{
#endif
#if defined(MBEDTLS_SSL_SESSION_TICKETS) && defined(MBEDTLS_SSL_CLI_C)
		mbedtls_ssl_session session;
		mbedtls_ssl_session_init( &session );
#if defined(MBEDTLS_VERSION_NUMBER) && (MBEDTLS_VERSION_NUMBER>=0x03010000)
		session.tls_version=MBEDTLS_SSL_VERSION_TLS1_2;
#endif
		if (ext_data) {
			session.ticket_len = ext_len;
			session.ticket = mbedtls_calloc( 1, ext_len );
			if (!session.ticket) {
				wpa_printf(MSG_ERROR, "TLS: ticket malloc error");
				return 0;
			}
			memcpy(session.ticket, ext_data, ext_len);
		} else {
			session.ticket_len = 0;
			session.ticket = NULL;
		}

		mbedtls_ssl_set_session(ssl, (const mbedtls_ssl_session *)&session );

		mbedtls_ssl_session_free(&session);

		return 1;
#endif
#if defined(MBEDTLS_VERSION_NUMBER) && (MBEDTLS_VERSION_NUMBER<0x03010000)
	}
#endif
	return 0;
}


int tls_connection_client_hello_ext(void *tls_ctx, struct tls_connection *conn,
					int ext_type, const u8 *data,
					size_t data_len)
{
	wpa_printf(MSG_DEBUG, "TLS: tls_connection_client_hello_ext");

	mbedtls_ssl_context *ssl = ((struct eap_tls *)tls_ctx)->ssl;

	if (conn == NULL || ssl == NULL || ext_type != 35)
			return -1;

	if (tls_set_session_ticket_ext(ssl, (void *) data,
						data_len) != 1)
		return -1;

	return 0;
}

int tls_connection_set_cipher_list(void *tls_ctx, struct tls_connection *conn,
				   u8 *ciphers)
{

	wpa_printf(MSG_DEBUG, "TLS: tls_connection_set_cipher_list");

	static int ciphersuites[10];
	u8 *c;
	int count;
	mbedtls_ssl_context *ssl = ((struct eap_tls *)tls_ctx)->ssl;

	if (conn == NULL || ssl == NULL || ciphers == NULL)
		return -1;

	c = ciphers;
	count = 0;
	while(*c != TLS_CIPHER_NONE){
		switch(*c){
#if defined(MBEDTLS_VERSION_NUMBER) && (MBEDTLS_VERSION_NUMBER<0x03010000)
			case TLS_CIPHER_RC4_SHA:
				ciphersuites[count++] = MBEDTLS_TLS_RSA_WITH_RC4_128_SHA; /* mbedtls v3 drop support for RC4 TLS ciphersuites */
				break;
#endif
			case TLS_CIPHER_AES128_SHA:
				ciphersuites[count++] = MBEDTLS_TLS_RSA_WITH_AES_128_CBC_SHA;
				break;
			case TLS_CIPHER_RSA_DHE_AES128_SHA:
				ciphersuites[count++] = MBEDTLS_TLS_DHE_RSA_WITH_AES_128_CBC_SHA;
				break;
			case TLS_CIPHER_ANON_DH_AES128_SHA:
				//Cipher not support in MbedTLS;
				break;
			case TLS_CIPHER_RSA_DHE_AES256_SHA:
				ciphersuites[count++] = MBEDTLS_TLS_DHE_RSA_WITH_AES_256_CBC_SHA;
				break;
			case TLS_CIPHER_AES256_SHA:
				ciphersuites[count++] = MBEDTLS_TLS_RSA_WITH_AES_256_CBC_SHA;
				break;
		}
		c++;
	}
	ciphersuites[count] = 0;

	if (!count) {
		wpa_printf(MSG_DEBUG, "TLS: No ciphers listed");
		return -1;
	}

	wpa_printf(MSG_DEBUG, "TLS: set %d cipher suites", count);

	mbedtls_ssl_conf_ciphersuites((mbedtls_ssl_config *) ssl->conf, (const int *)ciphersuites );

	return 0;
}


int tls_get_version(void *ssl_ctx, struct tls_connection *conn,
		    char *buf, size_t buflen)
{
	wpa_printf(MSG_DEBUG, "TLS: tls_get_version");

	const char *name;
	mbedtls_ssl_context *ssl = ((struct eap_tls *)ssl_ctx)->ssl;

	if (conn == NULL) {
		return -1;
	}

	name = mbedtls_ssl_get_version(ssl);
	if (name == NULL) {
		return -1;
	}

	os_strlcpy(buf, name, buflen);

	return 0;
}


int tls_get_cipher(void *tls_ctx, struct tls_connection *conn,
		   char *buf, size_t buflen)
{

	wpa_printf(MSG_DEBUG, "TLS: tls_get_cipher");
	const char *name;
	mbedtls_ssl_context *ssl = ((struct eap_tls *)tls_ctx)->ssl;

	if (conn == NULL || ssl == NULL)
		return -1;

	name = mbedtls_ssl_get_ciphersuite(ssl);
	if (name == NULL)
		return -1;

	os_strlcpy(buf, name, buflen);
	return 0;
}
#endif /*CONFIG_USE_POLARSSL*/

int tls_connection_resumed(void *tls_ctx, struct tls_connection *conn)
{
	/* To avoid gcc warnings */
	( void ) conn;
	( void ) tls_ctx;

	wpa_printf(MSG_DEBUG, "TLS: tls_connection_resumed");
	return 0;
}

int tls_connection_enable_workaround(void *tls_ctx,
				     struct tls_connection *conn)
{
	/* To avoid gcc warnings */
	( void ) conn;
	( void ) tls_ctx;

	wpa_printf(MSG_DEBUG, "TLS: tls_connection_enable_workaround");
	return -1;
}

int tls_connection_get_failed(void *tls_ctx, struct tls_connection *conn)
{
	/* To avoid gcc warnings */
	( void ) conn;
	( void ) tls_ctx;

	wpa_printf(MSG_DEBUG, "TLS: tls_connection_get_failed");
	return 0;
}


int tls_connection_get_read_alerts(void *tls_ctx, struct tls_connection *conn)
{
	/* To avoid gcc warnings */
	( void ) conn;
	( void ) tls_ctx;

	wpa_printf(MSG_DEBUG, "TLS: tls_connection_get_read_alerts");
	return 0;
}


int tls_connection_get_write_alerts(void *tls_ctx,
				    struct tls_connection *conn)
{
	/* To avoid gcc warnings */
	( void ) conn;
	( void ) tls_ctx;

	wpa_printf(MSG_DEBUG, "TLS: tls_connection_get_write_alerts");
	return 0;
}


int tls_get_library_version(char *buf, size_t buf_len)
{
	wpa_printf(MSG_DEBUG, "TLS: tls_get_library_version");
	return os_snprintf(buf, buf_len, "none");
}


void tls_connection_set_success_data(struct tls_connection *conn,
				     struct wpabuf *data)
{
	/* To avoid gcc warnings */
	( void ) conn;
	( void ) data;

	wpa_printf(MSG_DEBUG, "TLS: tls_connection_set_success_data");
}


void tls_connection_set_success_data_resumed(struct tls_connection *conn)
{
	/* To avoid gcc warnings */
	( void ) conn;
	
	wpa_printf(MSG_DEBUG, "TLS: tls_connection_set_success_data_resumed");
}


const struct wpabuf *
tls_connection_get_success_data(struct tls_connection *conn)
{
	/* To avoid gcc warnings */
	( void ) conn;
	
	wpa_printf(MSG_DEBUG, "TLS: tls_connection_get_success_data");
	return NULL;
}


void tls_connection_remove_session(struct tls_connection *conn)
{
	/* To avoid gcc warnings */
	( void ) conn;
	
	wpa_printf(MSG_DEBUG, "TLS: tls_connection_get_success_data");
}

