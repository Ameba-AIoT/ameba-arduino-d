#ifndef JPEG_ENCODER
#define JPEG_ENCODER
#include "freertos_service.h"
#include "memory_encoder.h"
#include "encoder_buffer_handler.h"
#include "isp_api.h"

#define JPEG_ROTATE_0  0
#define JPEG_ROTATE_90R 1
#define JPEG_ROTATE_90L 2

struct jpeg_parameter {
	u32 width;      /* Number of pixels/line in input image         */
	u32 height;     /* Number of lines in input image               */
	u32 qLevel;          /* Quantization level (0 - 9)                   */
	u32 frameType; /* Input frame YUV / RGB format     */
	u32 start;
	u32 ratenum;
	u32 jpeg_full_header;
        u32 rotation;
};

struct jpeg_context {
	void* encoder;
	struct jpeg_parameter jpeg_parm;
	struct encoder_list_head encoder_jpeg_lh;
	uint32_t source_addr;
	uint32_t y_addr;
	uint32_t uv_addr;
	uint32_t dest_addr;
	uint32_t dest_len;
	uint32_t dest_actual_len;
	struct isp_info isp_info_value;
	struct mem_info mem_info_value;
	void (*snapshot_cb)(void);
	void (*change_parm_cb)(void*);
};

void *jpeg_open();
int jpeg_encode(void *ctx);
int jpeg_release(void *ctx);
int jpeg_initial(void *ctx,struct jpeg_parameter *jpeg_parm);
int jpeg_init_param(struct jpeg_parameter *jpeg_parm);
void jpeg_info();

#endif