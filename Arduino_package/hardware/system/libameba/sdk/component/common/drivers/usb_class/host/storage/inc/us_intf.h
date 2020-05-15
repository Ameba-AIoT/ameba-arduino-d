#ifndef __US_INTF_H_
#define __US_INTF_H_

#include "basic_types.h"

typedef enum
{
  MSC_OK = 0,
  MSC_NOT_READY,
  MSC_W_PROTECT,
  MSC_ERROR,
}MSC_RESULT;

extern MSC_RESULT	us_init(void);
extern MSC_RESULT	us_isready (void);

extern MSC_RESULT 	us_getStatus(void);
extern MSC_RESULT	us_getmaxlun (u8* lun_num);
extern MSC_RESULT	us_unitisready (u8 lun);

extern MSC_RESULT	us_inquiry (u8 *pbuf);

extern MSC_RESULT	us_getcap(u32 *last_blk_addr, u32 *block_size);

extern MSC_RESULT	us_read_blocks( u8 *pbuf, u32 sector, u32 count);

extern MSC_RESULT	us_write_blocks( const u8 *pbuf, u32 sector, u32 count);


// indicate usb storage driver status
extern bool USB_STORAGE_READY;

#endif

