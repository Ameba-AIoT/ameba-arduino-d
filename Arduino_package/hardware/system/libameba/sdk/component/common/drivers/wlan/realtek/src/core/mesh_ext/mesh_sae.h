#ifndef _MESH_SAE_H_
#define _MESH_SAE_H_

#include "mbedtls/ecp.h"
#include "mbedtls/bignum.h"
#include "mbedtls/md.h"
#include <drv_types.h>

#define MBEDTLS_DIGEST_LENGTH	32
#define SAE_AUTH_COMMIT		1
#define SAE_AUTH_CONFIRM	2
#define SAE_AUTH_ALG		3

#define SAE_DEBUG_ERR           0x01
#define SAE_DEBUG_PROTOCOL_MSG  0x02
#define SAE_DEBUG_STATE_MACHINE 0x04
#define SAE_DEBUG_CRYPTO        0x08
#define SAE_DEBUG_CRYPTO_VERB   0x10
#define AMPE_DEBUG_CANDIDATES   0x20
#define MESHD_DEBUG             0x40
#define AMPE_DEBUG_FSM          0x80
#define AMPE_DEBUG_KEYS        0x100
#define AMPE_DEBUG_ERR         0x200
#define SAE_DEBUG_REKEY        0x400
#define SAE_DEBUG_MBED			0x800
#define sae_debug_mask			0x00


#define    SAE_MAX_EC_GROUPS    10
#define    SAE_MAX_PASSWORD_LEN 65
#define MESH_CIPHER_SUITE_CCMP		0x000FAC04

typedef struct group_def_ {
    unsigned short group_num;
    mbedtls_ecp_group *group;
    mbedtls_mpi *order;
    mbedtls_mpi *prime;
    char password[80];
    struct group_def_ *next;
} GD;

typedef struct _sae_config {
    int group[SAE_MAX_EC_GROUPS];
    int num_groups;
    char pwd[SAE_MAX_PASSWORD_LEN];
    GD *gd;
    int debug;
    int retrans;
    int pmk_expiry;
    int open_threshold;
    int blacklist_timeout;
    int giveup_threshold;
} SAEConfig;

struct ampe_config {
	unsigned int retry_timeout_ms;
	unsigned int holding_timeout_ms;
	unsigned int confirm_timeout_ms;
	unsigned int max_retries;
	//struct mesh_node *mesh;
};

int sae_initialize (const char *ifname, const __u8 *passphrase, __u16 passphrase_len);


#endif //_MESH_SAE_H_
