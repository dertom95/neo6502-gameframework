#ifndef __NG_MEM_H_
#define __NG_MEM_H_

#include <stdint.h>
#include <stdbool.h>
#include "api/ng_config.h"

#define MEM_USAGE_UNKNOWN       0
#define MEM_USAGE_PIXELBUFFER   1
#define MEM_USAGE_SPRITEBUFFER  2
#define MEM_USAGE_TILEMAP       3

#define MAX_DATAMOUNTS 3

#define MEM_MAX_SEGMENTS 3
#define MEMBLOCK_USAGE_MASK   (15 << 0) // bits 0-3 usage
#define MEMBLOCK_SEGMENT_MASK (7 << 4)  // bits 4-6 segmentid

//TODO: I really meant it well, creating something to keep memory under control and making 
//      it possible to wipe certain usagetypes.... in the end it is just born dead. but I still use it ;)

typedef struct ng_mem_segment_t {
    void* start;
    void* tip;
    uint32_t used;
    uint32_t size;
} ng_mem_segment_t;

typedef struct ng_mem_datablock_t {
    ng_mem_block_t mem;
    void* info;
} __attribute__((aligned(4))) ng_mem_datablock_t;


typedef struct ng_mem_datamount_t {
    uint8_t* source;
    uint16_t size;
    uint16_t destination;
    uint8_t page;
    uint8_t flags;
} __attribute__((aligned(4)))  ng_mem_datamount_t;

extern uint8_t datamount_amount;
extern ng_mem_datamount_t* datamounts[MAX_DATAMOUNTS];

// create a segment (see MEM_MAX_SEEGMENTS for the max amount). return the segment-id. max-size: 131056
uint8_t ng_mem_segment_create(void* start, uint32_t size);
uint32_t ng_mem_segment_space_left(uint8_t segment_id);
void ng_mem_segment_wipe(uint8_t segment_id);

/// @brief tries to allocate the specified amount of bytes in the specific segment (using default alignment:4)
/// @param segment_id 
/// @param size 
/// @return 
void* ng_mem_allocate(uint8_t segment_id,uint32_t size);

/// @brief 
/// @param segment_id 
/// @param size 
/// @param alignment 
/// @return 
void* ng_mem_allocate_with_alignment(uint8_t segment_id,uint32_t size,uint8_t alignment);

// allocates a block of memory in the specified segment. 
// sizes needs to be multiple of 8. max-siz: 131056 and/or free space in segment
// input:segment-id,size,usage_type[0-3] (meta data. no logical value)
//       pointer to ng_mem_block_t*
//       
// returns: true if allocation was successful. false if not
bool ng_mem_allocate_block(uint8_t segment_id,uint32_t size, uint8_t usage_type, ng_mem_block_t* block); 

/// @brief Mount the specified area in the 6502-mem
/// @param data_mount 
void ng_mem_add_datamount(ng_mem_datamount_t* data_mount);

//void ng_mem_mount_block(ng_mem_datablock_t* dtb, uint16_t destination);
void ng_mem_mount_block(ng_mem_block_t* data_block, uint16_t destination);

uint32_t ng_memblock_get_size(ng_mem_block_t* mem_block); 
uint8_t  ng_memblock_get_usage(ng_mem_block_t* mem_block); 
uint8_t ng_mem_block_get_segmentid(ng_mem_block_t* block);
// wipe the block with 0
void ng_memblock_wipe(ng_mem_block_t* mem_block);

#endif