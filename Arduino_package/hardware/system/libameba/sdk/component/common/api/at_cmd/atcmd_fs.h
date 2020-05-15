#ifndef __ATCMD_FS_H__
#define __ATCMD_FS_H__

#include "diag.h"
#define RTW_ENABLE_FATFS_INFO

#ifdef RTW_ENABLE_FATFS_INFO
	#define AT_INFO(fmt, args...)	DBG_8195A("\r\n" fmt,## args)
	#define AT_ERROR(fmt, args...)	DBG_8195A("\r\n" fmt,## args)
	#define AT_WARN(fmt, args...)	DBG_8195A("\r\n" fmt,## args)
#else
    #define AT_INFO(fmt, args...)
	#define AT_ERROR(fmt, args...)	DBG_8195A("\r\n" fmt,## args)
	#define AT_WARN(fmt, args...)
#endif


#endif
