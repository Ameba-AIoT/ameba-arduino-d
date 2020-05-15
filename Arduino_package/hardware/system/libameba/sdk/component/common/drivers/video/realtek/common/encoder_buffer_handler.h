#ifndef ENCODER_BUFFER_HANDLER_H
#define ENCODER_BUFFER_HANDLER_H
#include "osdep_service.h"

struct encoder_data{
	unsigned int	index;//index value
	int				size;
	unsigned char	*addr;//start address
	int				type;//for audio or video
	int				common_count;//if share the common buffer
	u32				timestamp;
	u32				hw_timestamp;
	_list			list_data;
};

struct encoder_list_head{
	void	*memory_ctx;
	_mutex	list_lock;
	_list	list;
};

struct mem_info{
	uint32_t	mem_total_size;
	uint32_t	mem_block_size;
	uint32_t	mem_frame_size;
};

struct enc_buf_data{
	u32		index;
	u32		y_addr;
	u32		uv_addr;
	u32		audio_addr;
	u32		sample_addr;
	u32		size;
	u32		timestamp;
	u32		hw_timestamp;
	_list	list;
};

struct enc_list_handle{
	_list	enc_idle_list;
	_list	enc_acti_list;
	_sema	enc_sema;
	_lock	enc_lock;
};
void enc_list_initial(struct enc_list_handle *enc_list);
void enc_list_deinitial(struct enc_list_handle *enc_list);
void enc_set_idle_buffer(struct enc_list_handle *enc_list,struct enc_buf_data *enc_data);
void enc_set_acti_buffer(struct enc_list_handle *enc_list,struct enc_buf_data *enc_data);
struct enc_buf_data *enc_get_idle_buffer(struct enc_list_handle *enc_list);
struct enc_buf_data *enc_get_acti_buffer(struct enc_list_handle *enc_list);
#endif