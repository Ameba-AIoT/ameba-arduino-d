#ifndef __FS_DEBUG_H__
#define __FS_DEBUG_H__

#include "diag.h"
#define RTW_ENABLE_FATFS_INFO

#ifdef RTW_ENABLE_FATFS_INFO
	#define FF_INFO(fmt, args...)	DBG_8195A("\r\n" fmt,## args)
	#define FF_ERROR(fmt, args...)	DBG_8195A("\r\n" fmt,## args)
	#define FF_WARN(fmt, args...)	DBG_8195A("\r\n" fmt,## args)
#else
    #define FF_INFO(fmt, args...)
	#define FF_ERROR(fmt, args...)	DBG_8195A("\r\n" fmt,## args)
	#define FF_WARN(fmt, args...)
#endif

#endif

