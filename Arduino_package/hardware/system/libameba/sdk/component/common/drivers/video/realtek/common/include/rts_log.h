/*
 * Realtek Semiconductor Corp.
 *
 * librtscamkit/rts_log.h
 *
 * Copyright (C) 2014      Ming Qian<ming_qian@realsil.com.cn>
 */
#ifndef _LIBRTSCAMKIT_RTS_LOG_H
#define _LIBRTSCAMKIT_RTS_LOG_H

#include <stdarg.h>

enum RTS_LOG_TYPE {
	RTS_LOG_IDX_CONS = 0,
	RTS_LOG_IDX_SYSLOG,
	RTS_LOG_IDX_FILE,
	RTS_LOG_IDX_CALLBACK,
	RTS_LOG_IDX_RESERVED
};

#define RTS_LOG_MASK_CONS		(0x1 << RTS_LOG_IDX_CONS)
#define RTS_LOG_MASK_SYSLOG		(0x1 << RTS_LOG_IDX_SYSLOG)
#define RTS_LOG_MASK_FILE		(0x1 << RTS_LOG_IDX_FILE)
#define RTS_LOG_MASK_CALLBACK		(0x1 << RTS_LOG_IDX_CALLBACK)

enum RTS_LOG_PRIORITY {
	RTS_LOG_DEBUG = 0,
	RTS_LOG_INFO,
	RTS_LOG_NOTICE,
	RTS_LOG_WARNING,
	RTS_LOG_ERR,
	RTS_LOG_CRIT,
	RTS_LOG_ALERT,
	RTS_LOG_EMERG,
	RTS_LOG_MEM,
	RTS_LOG_RESERVED
};

/*
 * rts_log :
 * */
void rts_log(int priority, const char *format, ...);

/*
 * rts_set_log_ident :
 * */
int rts_set_log_ident(const char *ident);

/*
 * rts_set_log_mask :
 *
 * @mask : If the mask argument is 0, the current log mask is not modified
 *
 * return value : the current log_mask
 *
 * */
int rts_set_log_mask(int mask);

/*
 * rts_set_log_file :
 *
 * @filename : NULL will close log file
 *
 * return value: 0 success, others fail
 *
 * */
int rts_set_log_file(const char *filename);

int rts_set_log_callback(void (*callback)(const char *, va_list));

/*
 * rts_set_log_level
 *
 * @level_mask : If the level_mask argument is 0, the current level_mask is not modified.
 *
 * return value : the current level_mask
 *
 * */
int rts_set_log_level(int level_mask);

/*
 * rts_dump_data :
 * */
void rts_dump_data(char *data, unsigned int len, unsigned int row);

#define RTS_DEBUG(...) \
	do {\
		rts_log(RTS_LOG_DEBUG, __VA_ARGS__);\
	} while (0)

#define RTS_INFO(...) \
	do {\
		rts_log(RTS_LOG_INFO, __VA_ARGS__);\
	} while (0)

#define RTS_NOTICE(...) \
	do {\
		rts_log(RTS_LOG_NOTICE, __VA_ARGS__);\
	} while (0)

#define RTS_WARNING(...) \
	do {\
		rts_log(RTS_LOG_WARNING, __VA_ARGS__);\
	} while (0)

#define RTS_ERR(...) \
	do {\
		rts_log(RTS_LOG_ERR, __VA_ARGS__);\
	} while (0)

#define RTS_CRIT(...) \
	do {\
		rts_log(RTS_LOG_CRIT, __VA_ARGS__);\
	} while (0)

#define RTS_ALERT(...) \
	do {\
		rts_log(RTS_LOG_ALERT, __VA_ARGS__);\
	} while (0)

#define RTS_EMERG(...) \
	do {\
		rts_log(RTS_LOG_EMERG, __VA_ARGS__);\
	} while (0)

#define RTS_OPT(...) \
	do {\
		fprintf(stdout, __VA_ARGS__);\
	} while (0)

#endif

