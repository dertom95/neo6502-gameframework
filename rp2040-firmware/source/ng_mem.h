#ifndef __NG_MEM_H_
#define __NG_MEM_H_

#include <stdint.h>
#include <stdbool.h>

#define MEM_USAGE_PIXELBUFFER 0
#define MEM_USAGE_TILESHEET 1
#define MEM_USAGE_TILEMAP 2

#define flags_set(FLAGS,MASK) FLAGS |= MASK;
#define flags_unset(FLAGS,MASK) FLAGS &= ~MASK;
#define flags_isset(FLAGS,MASK) ((FLAGS & MASK)==MASK)
#define flags_isset_some(FLAGS,MASK) ((FLAGS & MASK)>0)

#define MEM_MAX_SEGMENTS 3
#define MEMBLOCK_FLAG_INUSE (1 << 12)
#define MEMBLOCK_FLAG_MASK (7 << 12)
#define MEMBLOCK_INUSE(MEMBLOCK_PTR) (flags_isset(MEMBLOCK_PTR->start_pos,MEMBLOCK_FLAG_INUSE))
#define MEMBLOCK_SEGMENT_ID(MEMBLOCK_PTR) (MEMBLOCK_PTR->start_pos >> 13)
#define MEMBLOCK_USAGETYPE(MEMBLOCK_PTR) (MEMBLOCK_PTR->size >> 12)

typedef struct ng_mem_block_t {
    //TODO: start_pos really needed? I guess a uint8_t for flags and maybe uint8_t for userdata would be better    
    uint16_t start_pos; // bit 15-14: memory-segment   | bit13: block-inuse | bit 12-0: memory address (multiple of 8)
    uint16_t size;      // bit 15-13: usage-type(meta)                      | bit 12-0: size
    uint8_t* data;         // pointer to the actual data
} ng_mem_block_t;

typedef struct ng_mem_segment_t {
    void* start;
    void* tip;
    uint32_t used;
    uint32_t size;
} ng_mem_segment_t;

// create a segment (see MEM_MAX_SEEGMENTS for the max amount). return the segment-id. max-size: 131056
uint8_t ng_mem_segment_create(void* start, uint32_t size);
uint32_t ng_mem_segment_space_left(uint8_t segment_id);
void ng_mem_segment_wipe(uint8_t segment_id);

// allocates a block of memory in the specified segment. 
// sizes needs to be multiple of 8. max-siz: 131056 and/or free space in segment
// input:segment-id,size,usage_type[0-3] (meta data. no logical value)
//       pointer to ng_mem_block_t*
//       
// returns: true if allocation was successful. false if not
bool ng_mem_allocate(uint8_t segment_id,uint32_t size, uint8_t usage_type, ng_mem_block_t* block); 

uint32_t ng_memblock_get_size(ng_mem_block_t* mem_block); 
// wipe the block with 0
void ng_memblock_wipe(ng_mem_block_t* mem_block);

#endif