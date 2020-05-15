
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
static unsigned char  gDbgLevel = INIC_DBG_ERROR;
static unsigned int   gDbgFlag  = 0xFFFFFFFF;
#define INIC_PRINTK(fmt, args...)	printf(fmt"\r\n",## args)
#define _INIC_PRINTK(fmt, args...)	printf(fmt,## args)
#define INIC_DBG_MSG(flag, level, fmt, args...)					\
		do{														\
			if(((flag) & gDbgFlag) && (level <= gDbgLevel)){	\
				INIC_PRINTK(fmt,## args);							\
			}													\
		}while(0)
#else
#define INIC_PRINTK(fmt, args...)
#define INIC_DBG_MSG(flag, level, fmt, args...)	
#endif

#endif//__INIC_ETHERNETIF_H__