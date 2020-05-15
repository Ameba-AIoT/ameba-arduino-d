/*
 * Realtek Semiconductor Corp.
 *
 * include/rts_linux_lock.h
 *
 * Copyright (C) 2014      Ming Qian<ming_qian@realsil.com.cn>
 */
#ifndef _RTS_CAMKIT_H
#define _RTS_CAMKIT_H

#include <stdint.h>
#include <stddef.h>
//#include <sys/types.h>
//#include <drv_types.h>
#include <rts_errno.h>
#include <rts_log.h>
//#include <rts_linux_lock.h>

typedef long int off_t;

#ifdef RTS_FALSE
#undef RTS_FALSE
#endif
#define RTS_FALSE	0

#ifdef RTS_TRUE
#undef RTS_TRUE
#endif
#define RTS_TRUE	1

#ifndef RTS_L_BYTE
#define RTS_L_BYTE(a)		((a) & 0xff)
#endif
#ifndef RTS_H_BYTE
#define RTS_H_BYTE(a)		(((a) >> 8) & 0xff)
#endif
#ifndef RTS_L_WORD
#define RTS_L_WORD(a)		((a) & 0xffff)
#endif
#ifndef RTS_H_WORD
#define RTS_H_WORD(a)		(((a) >> 16) & 0xffff)
#endif


#ifndef RTS_MAKE_WORD
#define RTS_MAKE_WORD(a, b)	((a) | ((b) << 8))
#endif
#ifndef RTS_MAKE_DWORD
#define RTS_MAKE_DWORD(a, b, c, d)	\
	((a) | ((b) << 8) | ((c) << 16) | ((d) << 24))
#endif

#ifndef RTS_MAKE_WORD_BE
#define RTS_MAKE_WORD_BE(a, b)	((b) | ((a) << 8))
#endif
#ifndef RTS_MAKE_DWORD_BE
#define RTS_MAKE_DWORD_BE(a, b, c, d)	\
	((d) | ((c) << 8) | ((b) << 16) | ((a) << 24))
#endif

#ifndef RTS_SET_BIT
#define RTS_SET_BIT(v, i)	((v) | (1<<(i)))
#endif
#ifndef RTS_CLEAR_BIT
#define RTS_CLEAR_BIT(v, i)	((v) & (~(1<<(i))))
#endif
#ifndef RTS_CHECK_BIT
#define RTS_CHECK_BIT(v, i)	(((v)>>(i)) & 1)
#endif

#ifndef RTS_SAFE_DELETE
#define RTS_SAFE_DELETE(p)	\
		do { \
			if (p)\
				rts_free(p);\
			p = NULL;\
		} while (0)
#endif

#ifndef RTS_ARRAY_SIZE
#define RTS_ARRAY_SIZE(array)	(sizeof(array) / sizeof((array)[0]))
#endif

#define RTS_YUV_HEADER_SIZE			128

#ifndef offsetof
#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)
#endif

/**
 * container_of - cast a member of a structure out to the containing structure
 * @ptr:	the pointer to the member.
 * @type:	the type of the container struct this is embedded in.
 * @member:	the name of the member within the struct.
 *
 */
#ifndef container_of
#define container_of(ptr, type, member) ({			\
	const typeof( ((type *)0)->member ) *__mptr = (ptr);	\
	(type *)( (char *)__mptr - offsetof(type,member) );})
#endif

struct rts_version_t {
	unsigned int major;
	unsigned int minor;
	unsigned int build;
	unsigned int reserved;
};

int rts_is_integer(const char *s);
off_t rts_get_file_size(const char *filename);
int rts_save_raw_data(const char *name, uint8_t *data, int length);
int rts_save_yuv_image(const char *name, uint8_t *data,
		uint32_t width, uint32_t height, int length);

void *rts_malloc(size_t size);
void rts_free(void *ptr);
void *rts_calloc(size_t nmemb, size_t size);
void *rts_realloc(void *ptr, size_t size);

#endif

