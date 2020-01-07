#include "../gsm610/gsm.h"
#define SYMBOL_ARRAY_SIZE	8000
#define		WAVLIKE_GSM610_BLOCKSIZE	65
#define		WAVLIKE_GSM610_SAMPLES		320
#define		DICT_ARRAY_CHINESE_BASE				0x4e00
#define		DICT_ARRAY_SIZE					0x5200
#define		GSM_WAV_HEADER_LEN			60

#if 0
typedef struct _phonetic_symbol_
{
	int offset;
	int bytes;
}PhoneticSymbol;

typedef struct _dict_item_
{
	int code;
	PhoneticSymbol *phonSymbol;
}DictItem;	
#else

typedef struct _phonetic_symbol_
{
	int offset;
	int bytes;
}PhoneticSymbol;

typedef struct _dict_item_
{
	short code;
	short symbolCode;
}DictItem;	
#endif
	
typedef struct gsm610_tag
{	int				blocks;
	int				blockcount, samplecount;
	int				samplesperblock, blocksize;

	short			samples [WAVLIKE_GSM610_SAMPLES];
	unsigned char	block [WAVLIKE_GSM610_BLOCKSIZE];

	/* Damn I hate typedef-ed pointers; yes, gsm is a pointer type. */
	gsm				gsm_data;
} GSM610_PRIVATE;