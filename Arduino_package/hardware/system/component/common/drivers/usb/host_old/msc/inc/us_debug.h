#ifndef _US_DEBUG_H_
#define _US_DEBUG_H_

#include <platform_opts.h>

#ifdef CONFIG_USBH_MSC

#include "diag.h"
#include "scsi/scsi_cmnd.h"

#define US_DEBUG    0
#define US_DRIVER	"MSC"

#if US_DEBUG
#define US_INFO(fmt, args...)		            printf("\n\r[%s]%s: " fmt, US_DRIVER, __FUNCTION__, ## args)
#define US_ERR(fmt, args...)		            printf("\n\r[%s]%s: " fmt, US_DRIVER,__FUNCTION__, ## args)
#define US_WARN(fmt, args...)		            printf("\n\r[%s]%s: " fmt, US_DRIVER,__FUNCTION__, ## args)
#define US_ENTER                                printf("\n\r[%s]%s ==>\n", US_DRIVER,__FUNCTION__)
#define US_EXIT                                 printf("\n\r[%s]%s <==\n", US_DRIVER,__FUNCTION__)
#else
#define US_INFO(fmt, args...)
#define US_ERR(fmt, args...)		            printf("\n\r[%s]%s: " fmt, US_DRIVER,__FUNCTION__, ## args)
#define US_WARN(fmt, args...)
#define US_ENTER
#define US_EXIT
#endif

#endif // CONFIG_USBH_MSC

#endif // _US_DEBUG_H_
