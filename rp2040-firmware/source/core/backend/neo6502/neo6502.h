#ifndef __NEO6502_H_
#define __NEO6502_H_

#include "../../../ng_mem.h"

#include <stdint.h>

void neo6502_copy_from_flash_to_ram(ng_mem_block_t* block, uint8_t segment_id,uint8_t usage_type,void* data,uint32_t size);

#endif