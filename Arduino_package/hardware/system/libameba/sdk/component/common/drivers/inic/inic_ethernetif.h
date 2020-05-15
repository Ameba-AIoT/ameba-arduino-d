
#ifndef __INIC_ETHERNETIF_H__
#define __INIC_ETHERNETIF_H__

#define INIC_BIT(n)           (1<<n)
#define INIC_FLAG        INIC_BIT(0)
#define INIC_FLAG_SDIO        INIC_BIT(1)
#define INIC_FLAG_USB        INIC_BIT(2)
#define INIC_FLAG_SPI         INIC_BIT(3)

enum{
	INIC_DBG_OFF = 0,
	INIC_DBG_ALWAYS,
	INIC_DBG_ERROR,
	INIC_DBG_WARNING,
	INIC_DBG_INFO
};

#define INIC_DBG

#ifdef INIC_DBG
static unsigned char  gINICDbgLevel = INIC_DBG_ERROR;
static unsigned int   gINICDbgFlag  = 0xFFFFFFFF;
#define INIC_PRINTK(fmt, args...)	printf(fmt"\r\n",## args)
#define _INIC_PRINTK(fmt, args...)	printf(fmt,## args)
#define INIC_DBG_MSG(flag, level, fmt, args...)					\
		do{														\
			if(((flag) & gINICDbgFlag) && (level <= gINICDbgLevel)){	\
				INIC_PRINTK(fmt,## args);							\
			}													\
		}while(0)
extern _LONG_CALL_ void __rtl_memDump_v1_00(const u8 *start, u32 size, char * strHeader);
#define INIC_DUMP(pData, Len) __rtl_memDump_v1_00(pData, Len, NULL)
#else
#define INIC_PRINTK(fmt, args...)
#define INIC_DBG_MSG(flag, level, fmt, args...)	
#define INIC_DUMP(pData, Len)
#endif

#ifndef inic_memcpy
	#define inic_memcpy(d, s, n)	rtw_memcpy((void*)(d), ((void*)(s)), (n))
#endif

#ifndef inic_malloc
	#define inic_malloc(sz)	rtw_malloc(sz)
#endif

#ifndef inic_zmalloc
	#define inic_zmalloc(sz)	rtw_zmalloc(sz)
#endif

#ifndef inic_memset
	#define inic_memset(pbuf, c, sz)	rtw_memset(pbuf, c, sz)
#endif

#ifndef inic_mfree
	#define inic_mfree(p, sz)	rtw_mfree(((u8*)(p)), (sz))
#endif

#ifndef inic_memcmp
	#define inic_memcmp(s1, s2, n) rtw_memcmp(((void*)(s1)), ((void*)(s2)), (n))
#endif

#ifndef inic_mdelay_os
	#define inic_mdelay_os(ms) rtw_mdelay_os(ms)
#endif
#endif//__INIC_ETHERNETIF_H__