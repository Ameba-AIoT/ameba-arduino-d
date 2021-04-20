#include "cmsis.h"
#include "platform_stdlib.h"

#include "mbedtls/config.h"
#include "mbedtls/platform.h"
#include "mbedtls/ssl.h"
#include "mbedtls/debug.h"

extern const unsigned char *client_cert_s;

extern const unsigned char *ca_cert_s;

static void _debug(void *ctx, int level, const char *file, int line, const char *str)
{
	/* To avoid gcc warnings */
	( void ) ctx;
	( void ) level;
	
	printf("\n\r%s:%d: %s\n\r", file, line, str);
}

static int _verify(void *data, mbedtls_x509_crt *crt, int depth, uint32_t *flags) 
{
	char buf[1024];
	((void) data);

	printf("Verify requested for (Depth %d):\n", depth);
	mbedtls_x509_crt_info(buf, sizeof(buf) - 1, "", crt);
	printf("%s", buf);

	if(((*flags) & MBEDTLS_X509_BADCERT_EXPIRED) != 0)
		printf("server certificate has expired\n");

	if(((*flags) & MBEDTLS_X509_BADCERT_REVOKED) != 0)
		printf("  ! server certificate has been revoked\n");

	if(((*flags) & MBEDTLS_X509_BADCERT_CN_MISMATCH) != 0)
		printf("  ! CN mismatch\n");

	if(((*flags) & MBEDTLS_X509_BADCERT_NOT_TRUSTED) != 0)
		printf("  ! self-signed or not signed by a trusted CA\n");

	if(((*flags) & MBEDTLS_X509_BADCRL_NOT_TRUSTED) != 0)
		printf("  ! CRL not trusted\n");

	if(((*flags) & MBEDTLS_X509_BADCRL_EXPIRED) != 0)
		printf("  ! CRL expired\n");

	if(((*flags) & MBEDTLS_X509_BADCERT_OTHER) != 0)
		printf("  ! other (unknown) flag\n");

	if((*flags) == 0)
		printf("  Certificate verified without error flags\n");

	return(0);
}

#if defined(__ICCARM__)
void* (__cmse_nonsecure_call *ns_calloc)(size_t, size_t) = NULL;
void (__cmse_nonsecure_call *ns_free)(void *) = NULL;
#else
void* __attribute__((cmse_nonsecure_call)) (*ns_calloc)(size_t, size_t) = NULL;
void __attribute__((cmse_nonsecure_call)) (*ns_free)(void *) = NULL;
#endif

int NS_ENTRY secure_mbedtls_platform_set_ns_calloc_free(
	void* (*calloc_func)(size_t, size_t),
	void (*free_func)(void *))
{
#if defined(__ICCARM__)
    ns_calloc = cmse_nsfptr_create((void* (__cmse_nonsecure_call *)(size_t, size_t)) calloc_func);
    ns_free = cmse_nsfptr_create((void (__cmse_nonsecure_call *)(void *)) free_func);
#else
    ns_calloc = cmse_nsfptr_create((void* __attribute__((cmse_nonsecure_call)) (*)(size_t, size_t)) calloc_func);
    ns_free = cmse_nsfptr_create((void __attribute__((cmse_nonsecure_call)) (*)(void *)) free_func);
#endif
    return( 0 );
}

void NS_ENTRY secure_mbedtls_ssl_init(mbedtls_ssl_context *ssl)
{
	mbedtls_ssl_init(ssl);
}

void NS_ENTRY secure_mbedtls_ssl_conf_dbg(mbedtls_ssl_config *conf, void  *p_dbg)
{
	mbedtls_debug_set_threshold(0);
	mbedtls_ssl_conf_dbg(conf, _debug, p_dbg);
}

void NS_ENTRY secure_mbedtls_ssl_conf_verify(mbedtls_ssl_config *conf, void *p_vrfy)
{
	mbedtls_ssl_conf_verify(conf, _verify, p_vrfy);
}

int NS_ENTRY secure_mbedtls_ssl_conf_own_cert(
	mbedtls_ssl_config *conf,
	mbedtls_x509_crt *own_cert,
	mbedtls_pk_context *pk_key)
{
	return mbedtls_ssl_conf_own_cert(conf, own_cert, pk_key);
}

mbedtls_x509_crt* NS_ENTRY secure_mbedtls_x509_crt_parse(void)
{
	mbedtls_x509_crt *client_crt = (mbedtls_x509_crt *) mbedtls_calloc(1, sizeof(mbedtls_x509_crt));

	if(client_crt) {
		mbedtls_x509_crt_init(client_crt);

		if(mbedtls_x509_crt_parse(client_crt, client_cert_s, strlen((char const*) client_cert_s) + 1) != 0) {
			printf("\n\r ERROR: mbedtls_x509_crt_parse \n\r");
			goto error;
		}
	}
	else {
		printf("\n\r ERROR: mbedtls_calloc \n\r");
		goto error;
	}

	return client_crt;

error:
	if(client_crt) {
		mbedtls_x509_crt_free(client_crt);
		mbedtls_free(client_crt);
	}

	return NULL;
}

mbedtls_x509_crt* NS_ENTRY secure_mbedtls_x509_crt_parse_ca(void)
{
	mbedtls_x509_crt *ca_crt = (mbedtls_x509_crt *) mbedtls_calloc(1, sizeof(mbedtls_x509_crt));

	if(ca_crt) {
		mbedtls_x509_crt_init(ca_crt);

		if(mbedtls_x509_crt_parse(ca_crt, ca_cert_s, strlen((char const*) ca_cert_s) + 1) != 0) {
			printf("\n\r ERROR: mbedtls_x509_crt_parse \n\r");
			goto error;
		}
	}
	else {
		printf("\n\r ERROR: mbedtls_calloc \n\r");
		goto error;
	}

	return ca_crt;

error:
	if(ca_crt) {
		mbedtls_x509_crt_free(ca_crt);
		mbedtls_free(ca_crt);
	}

	return NULL;
}

void NS_ENTRY secure_mbedtls_x509_crt_free(mbedtls_x509_crt *crt)
{
	mbedtls_x509_crt_free(crt);
	mbedtls_free(crt);
}

int NS_ENTRY secure_mbedtls_ssl_setup(mbedtls_ssl_context *ssl, const mbedtls_ssl_config *conf)
{
	return mbedtls_ssl_setup(ssl, conf);
}

void NS_ENTRY secure_mbedtls_ssl_free(mbedtls_ssl_context *ssl)
{
	mbedtls_ssl_free(ssl);
}

void NS_ENTRY secure_mbedtls_ssl_config_free(mbedtls_ssl_config *conf)
{
	mbedtls_ssl_config_free(conf);
}

int NS_ENTRY secure_mbedtls_ssl_handshake(mbedtls_ssl_context *ssl)
{
	return mbedtls_ssl_handshake(ssl);
}

char* NS_ENTRY secure_mbedtls_ssl_get_ciphersuite(const mbedtls_ssl_context *ssl, char *buf)
{
	strcpy(buf, mbedtls_ssl_get_ciphersuite(ssl));
	return buf;
}

int NS_ENTRY secure_mbedtls_ssl_close_notify(mbedtls_ssl_context *ssl)
{
	return mbedtls_ssl_close_notify(ssl);
}

int NS_ENTRY secure_mbedtls_ssl_read(mbedtls_ssl_context *ssl, unsigned char *buf, size_t len)
{
	return mbedtls_ssl_read(ssl, buf, len);
}

int NS_ENTRY secure_mbedtls_ssl_write(mbedtls_ssl_context *ssl, const unsigned char *buf, size_t len)
{
	return mbedtls_ssl_write(ssl, buf, len);
}
