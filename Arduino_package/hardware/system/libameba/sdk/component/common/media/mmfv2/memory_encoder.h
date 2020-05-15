#ifndef ENCODER_MEMORY_H
#define ENCODER_MEMORY_H

#include "osdep_service.h"
#include <stdint.h>

struct encoder_buffer{
		_mutex memory_lock;
        
        uint8_t* pool;
        uint8_t* pool_table;     // index table
        int32_t max_pool_blk;   // 
        
        int32_t blk_size;       // one block size 
        int32_t mem_size;       // total pool size
        
        int32_t pool_blk_idx;
		int32_t tail_blk_idx;	// tail empty block start index, it will set when pool_blk_idx reset to 0, clear to -1, when pool_blk_idx != 0 in realloc function
        int32_t rest_pool_blk;
        
        int32_t blk_serial;     
        
        int32_t min_rest_blk;   
};


void *memory_init(int size,int block_size);
void memory_deinit(void *ctx);
//u8* _memory_realloc(struct encoder_buffer *eb,uint8_t *addr,uint32_t size);
u8* memory_realloc(void *ctx,uint8_t *addr,uint32_t size);
void memory_free(void *ctx,uint8_t *addr);
void* memory_alloc(void *ctx,uint32_t size);
void dump_info(void *ctx);

#endif