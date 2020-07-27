/*************************************************************************************************/
//qr_code_scanner.h
/************************************************************************************************/
#ifndef __QR_CODE_SCANNER_H__
#define __QR_CODE_SCANNER_H__

#define QR_CODE_MAX_RESULT_LENGTH	256

//DEBUG SETTING
#define QR_CODE_ERROR		0
#define QR_CODE_WARNING		1
#define QR_CODE_INFO		2
#define QR_CODE_DEBUG		3
#define QR_CODE_NONE		0xFF
#define QR_CODE_DEBUG_LEVEL QR_CODE_INFO

#if (QR_CODE_DEBUG_LEVEL == QR_CODE_NONE)
#define qr_code_printf(level, fmt, arg...)
#else
#define qr_code_printf(level, fmt, arg...)     \
do {\
	if (level <= QR_CODE_DEBUG_LEVEL) {\
		if (level <= QR_CODE_ERROR) {\
			printf("\r\nERROR: " fmt, ##arg);\
		} \
		else {\
			printf("\r\n"fmt, ##arg);\
		} \
	}\
}while(0)
#endif

typedef enum {
	QR_CODE_SUCCESS = 0,
	QR_CODE_FAIL_UNSPECIFIC_ERROR = 1,
	QR_CODE_FAIL_NO_FINDER_CENTER = 2,
	QR_CODE_FAIL_DECODE_ERROR = 3
}qr_code_scanner_result;

/*
 * @brief               This function parses the image which contains a QR code and returns the result string if parsing success.
 * @param [raw_data]:   The pointer to the raw data of the luminance value of the image to be parsed.
 * @param [width]:      The width of the image to be parsed.
 * @param [height]:     The height of the image to be parsed.
 * @param [x_density]:  The vertical scan density.
 * @param [y_density]:  The horizontal scan density. 
 * @param [string]:     The pointer to the result string.
 * @param [len]:        The pointer to the length of the result string.
 * @return              The parsing result of the image.
 */
qr_code_scanner_result qr_code_parsing(unsigned char *raw_data, unsigned int width, unsigned int height, int x_density, int y_density, unsigned char *string, unsigned int *len);

#endif

