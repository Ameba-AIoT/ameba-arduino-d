#ifndef _US_SCSI_H
#define _US_SCSI_H

#include <platform_opts.h>

#ifdef CONFIG_USBH_MSC

/**
 * define transfer length
 */
#define TRANS_LEN_READ_10			512
#define TRANS_LEN_WRITE_10			512
#define TRANS_LEN_INQUIRY			36
#define TRANS_LEN_TEST_UNIT_READY	0
#define TRANS_LEN_READ_CAPACITY_10	8
#define TRANS_LEN_READ_CAPACITY_16	12
#define TRANS_LEN_REQUEST_SENSE		18
#define TRANS_LEN_MODE_SENSE		192

extern int scsi_cmnd_execute(char cmnd, unsigned char * _buff,
						unsigned long _sector, unsigned int _count);

#endif // CONFIG_USBH_MSC

#endif // _US_SCSI_H

