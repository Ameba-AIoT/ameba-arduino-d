#ifndef _BT_TAG_SCANNER_KEEP_MESSAGE_H_
#define _BT_TAG_SCANNER_KEEP_MESSAGE_H_

#include "stdint.h"
#include "stdbool.h"


#define BT_TAG_SCANNER_RPL_ENTRY_NUM_MAX 30


typedef struct
{
	uint8_t addr[6];
}rpl_mac_t;

typedef struct
{
	uint8_t addr[6];
	uint8_t seq[4];
}tag_info_t;

typedef struct
{
	uint8_t scanbuffer[14];
	uint8_t savebuffer[10];
	uint8_t sendbuffer[240];
	uint8_t senddatalen;
	uint32_t scanseq;
	uint8_t saveseq[4];
	uint8_t saveflag;

}tag_t;

#define BE_BUFFER_TO_UINT32(u32, a)  {                   \
        u32 = ((uint32_t)(*(a + 0)) << 0) +            \
              ((uint32_t)(*(a + 1)) << 8) +            \
              ((uint32_t)(*(a + 2)) << 16) +            \
              ((uint32_t)(*(a + 3)) << 24);             \
    }

void rpl_init(uint16_t entry_num);
void rpl_deinit(void);
int rpl_check(rpl_mac_t scan_mac, uint32_t seq, bool update);
void rpl_clear(void);
void bt_tag_scanner_flash_restore(void);
void bt_tag_scanner_flash_init(void);
#endif
