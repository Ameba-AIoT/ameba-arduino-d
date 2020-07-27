#ifndef _GOOGLE_CLOUD_IOT_H_
#define _GOOGLE_CLOUD_IOT_H_

#include "MQTTClient.h"

#define GOOGLECLOUD_TLS_POLARSSL       0    /*!< Use PolarSSL for TLS when GOOGLECLOUD */
#define GOOGLECLOUD_TLS_MBEDTLS        1    /*!< Use mbedTLS for TLS when GOOGLECLOUD */

#if CONFIG_USE_POLARSSL
#define GOOGLECLOUD_USE_TLS            GOOGLECLOUD_TLS_POLARSSL
#elif CONFIG_USE_MBEDTLS
#define GOOGLECLOUD_USE_TLS            GOOGLECLOUD_TLS_MBEDTLS
#endif




#define DEBUG_GOOGLECLOUD	1


#define GOOGLECLOUD_LOG(level, fmt, ...) printf("\n\r[GOOGLECLOUD %s] %s: " fmt "\n", level, __FUNCTION__, ##__VA_ARGS__)

#if DEBUG_GOOGLECLOUD == 2
#define GOOGLECLOUD_DEBUG(fmt, ...) GOOGLECLOUD_LOG("DEBUG", fmt, ##__VA_ARGS__)
#else
#define GOOGLECLOUD_DEBUG(fmt, ...)
#endif
#if DEBUG_GOOGLECLOUD
#define GOOGLECLOUD_ERROR(fmt, ...) GOOGLECLOUD_LOG("ERROR", fmt, ##__VA_ARGS__)
#else
#define GOOGLECLOUD_ERROR(fmt, ...)
#endif

typedef struct {
	char *sig;
	int length;
}jwt_sign;

typedef struct _GoogleCloud_context{
	MQTTClient client;
	Network network;
	char *project_id;
	char *registry_id;
	char *device_id;
} GoogleCloud_context;

char *jwt_generator(const unsigned char *private_key, char *project_id, int expiration_seconds);
int gc_client_connect(GoogleCloud_context *gc_client, const unsigned char *private_key);
int gc_publish(GoogleCloud_context *gc_client, char *payload, int payload_len, int message_num);
int gc_client_init(GoogleCloud_context *gc_client, char *project_id, char *registry_id, char *device_id, int send_buf_len, int read_buf_len);
void gc_client_close(GoogleCloud_context *gc_client);

#endif
