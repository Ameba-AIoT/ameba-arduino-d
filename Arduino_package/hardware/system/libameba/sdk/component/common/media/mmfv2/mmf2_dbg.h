#ifndef __MMF_DBG_H_
#define __MMF_DBG_H_

#include <stdint.h>

// Define debug group
#define	_MMF_DBG_RTSP_          	0x00000001
#define	_MMF_DBG_RTP_          		0x00000002
#define	_MMF_DBG_ISP_			    0x00000004
#define	_MMF_DBG_ENCODER_			0x00000008

extern uint32_t ConfigDebugMmfErr;
extern uint32_t ConfigDebugMmfWarn;
extern uint32_t ConfigDebugMmfInfo;

#define DBG_MMF_ERR_MSG_ON(x)       (ConfigDebugMmfErr |= (x))
#define DBG_MMF_WARN_MSG_ON(x)      (ConfigDebugMmfWarn |= (x))
#define DBG_MMF_INFO_MSG_ON(x)      (ConfigDebugMmfInfo |= (x))

#define DBG_MMF_ERR_MSG_OFF(x)      (ConfigDebugMmfErr &= ~(x))
#define DBG_MMF_WARN_MSG_OFF(x)     (ConfigDebugMmfWarn &= ~(x))
#define DBG_MMF_INFO_MSG_OFF(x)     (ConfigDebugMmfInfo &= ~(x))


#define RTSP_ERR_PREFIX         "[RTSP Err]"
#define RTSP_WARN_PREFIX        "[RTSP Wrn]"
#define RTSP_INFO_PREFIX        "[RTSP Inf]"

#define RTP_ERR_PREFIX         "[RTP Err]"
#define RTP_WARN_PREFIX        "[RTP Wrn]"
#define RTP_INFO_PREFIX        "[RTP Inf]"

#define ISP_ERR_PREFIX         "[ISP Err]"
#define ISP_WARN_PREFIX        "[ISP Wrn]"
#define ISP_INFO_PREFIX        "[ISP Inf]"

#define ENCODER_ERR_PREFIX         "[Enc Err]"
#define ENCODER_WARN_PREFIX        "[Enc Wrn]"
#define ENCODER_INFO_PREFIX        "[Enc Inf]"


#define MMF_PRINTK      printf

#define RTSP_DBG_ERROR(...)													\
			do {															\
				if (ConfigDebugMmfErr&_MMF_DBG_RTSP_)						\
					MMF_PRINTK("\n\r" RTSP_ERR_PREFIX __VA_ARGS__);			\
			}while(0)
#define RTP_DBG_ERROR(...)													\
			do {															\
				if (ConfigDebugMmfErr&_MMF_DBG_RTP_)						\
					MMF_PRINTK("\n\r" RTP_ERR_PREFIX __VA_ARGS__);			\
			}while(0)
#ifdef ISP_DBG_ERROR
#undef ISP_DBG_ERROR                          
#define ISP_DBG_ERROR(...)													\
			do {															\
				if (ConfigDebugMmfErr&_MMF_DBG_ISP_)						\
					MMF_PRINTK("\n\r" ISP_ERR_PREFIX __VA_ARGS__);			\
			}while(0)
#endif
#define ENCODER_DBG_ERROR(...)												\
			do {															\
				if (ConfigDebugMmfErr&_MMF_DBG_ENCODER_)					\
					MMF_PRINTK("\n\r" ENCODER_ERR_PREFIX __VA_ARGS__);		\
			}while(0)

// DBG_WARNING
#define RTSP_DBG_WARNING(...)												\
			do {															\
				if (ConfigDebugMmfWarn&_MMF_DBG_RTSP_)						\
					MMF_PRINTK("\n\r" RTSP_WARN_PREFIX __VA_ARGS__);		\
			}while(0)
#define RTP_DBG_WARNING(...)												\
			do {															\
				if (ConfigDebugMmfWarn&_MMF_DBG_RTP_)						\
					MMF_PRINTK("\n\r" RTP_WARN_PREFIX __VA_ARGS__);			\
			}while(0)
#define ISP_DBG_WARNING(...)												\
			do {															\
				if (ConfigDebugMmfWarn&_MMF_DBG_ISP_)						\
					MMF_PRINTK("\n\r" ISP_WARN_PREFIX __VA_ARGS__);			\
			}while(0)
#define ENCODER_DBG_WARNING(...)											\
			do {															\
				if (ConfigDebugMmfWarn&_MMF_DBG_ENCODER_)					\
					MMF_PRINTK("\n\r" ENCODER_WARN_PREFIX __VA_ARGS__);		\
			}while(0)
				
// DBG_INFO
#define RTSP_DBG_INFO(...)													\
			do {															\
				if (ConfigDebugMmfInfo&_MMF_DBG_RTSP_)						\
					MMF_PRINTK("\n\r" RTSP_INFO_PREFIX __VA_ARGS__);		\
			}while(0)
#define RTP_DBG_INFO(...)													\
			do {															\
				if (ConfigDebugMmfInfo&_MMF_DBG_RTP_)						\
					MMF_PRINTK("\n\r" RTP_INFO_PREFIX __VA_ARGS__);			\
			}while(0)
#define ISP_DBG_INFO(...)													\
			do {															\
				if (ConfigDebugMmfInfo&_MMF_DBG_ISP_)						\
					MMF_PRINTK("\n\r" ISP_INFO_PREFIX __VA_ARGS__);			\
			}while(0)
#define ENCODER_DBG_INFO(...)												\
			do {															\
				if (ConfigDebugMmfInfo&_MMF_DBG_ENCODER_)					\
					MMF_PRINTK("\n\r" ENCODER_INFO_PREFIX __VA_ARGS__);		\
			}while(0)
#endif

