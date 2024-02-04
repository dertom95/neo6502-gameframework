#include "ng_mem.h"
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>

// the memory segments
uint8_t          mem_segments_amount=0;
ng_mem_segment_t mem_segments[MEM_MAX_SEGMENTS]={0};

uint32_t ng_memblock_get_startpos(ng_mem_block_t* mem_block)
{
    return mem_block->start_pos << 3;
}

// create a segment (see MEM_MAX_SEEGMENTS for the max amount). return the segment-id. max-size: 131056
uint8_t ng_mem_segment_create(void* start, uint32_t size)
{
    assert( (mem_segments_amount!= MEM_MAX_SEGMENTS) && "exceeded max amount of segments");
    assert( (((size_t)start)%16==0) && "ng_mem_segment: start-pos must be 8byte-aligned");
    ng_mem_segment_t* segment = &mem_segments[mem_segments_amount];
    segment->start = start;
    segment->tip = start;
    segment->used = 0;
    segment->size = size;
    return mem_segments_amount++;
}

uint32_t ng_mem_segment_space_left(uint8_t segment_id)
{
    assert(segment_id < mem_segments_amount);
    ng_mem_segment_t* segment = &mem_segments[segment_id];
    return segment->size - segment->used;
}

void ng_mem_segment_wipe(uint8_t segment_id)
{
    assert(segment_id < mem_segments_amount);
    ng_mem_segment_t* segment = &mem_segments[segment_id];
    memset(segment->start,0,segment->size);
}


#define BUILD_STARTPOS(SEG_POS,SEGMENT_ID) ((uint16_t)(SEG_POS >> 4)) | MEMBLOCK_FLAG_INUSE | (((uint16_t)SEGMENT_ID)<<13)
#define BUILD_SIZE(SIZE,USAGE) ((uint16_t)(SIZE >> 4)) | (((uint16_t)USAGE)<<13)

// allocates a block of memory in the specified segment. sizes needs to be multiple of 8. max-siz: 131056 and/or free space in segment
bool ng_mem_allocate(uint8_t segment_id,uint32_t size, uint8_t usage_type, ng_mem_block_t* block)
{
    assert(segment_id < mem_segments_amount && "unknown segment_id");
    assert(size < 131056 && "ng_allocte: exceeded size. size must not be >= 131056");
    assert(block!=NULL && "ng_allocate: you need to provide an output-block");

    ng_mem_segment_t* segment = &mem_segments[segment_id];
    assert( (((size_t)segment->tip)%16==0) && "ng_mem_segment: internal error: tip not 16byte-aligned");
    
    uint32_t space_left = segment->size-segment->used;
    if (space_left-size >= 16){
        // only align size if there is space left afterwards
        size += 16-(size%16); // make the size dividable by 8    
    }
    assert(space_left >= size && "exceeded segment-memory");
    block->data = segment->tip;
    block->start_pos = BUILD_STARTPOS(segment->used,segment_id);
    assert(usage_type<7 && "exceeded usage-type");
    block->size = BUILD_SIZE(size, usage_type);
    
    segment->tip += size;
    segment->used += size;
    return true;
}

uint8_t  ng_memblock_get_usage(ng_mem_block_t* mem_block)
{
    uint8_t result = (uint8_t)(mem_block->size >> 13);
    return result;
}


uint32_t ng_memblock_get_size(ng_mem_block_t* mem_block)
{
    uint32_t result = (mem_block->size & ~MEMBLOCK_FLAG_MASK)  << 4;
    return result;
}

// wipe the block with 0
void ng_memblock_wipe(ng_mem_block_t* mem_block)
{
    uint32_t size = ng_memblock_get_size(mem_block);
    memset(mem_block->data,0,size);
}

#if defined(TESTING)
// ▀█▀ █▀▀ █▀ ▀█▀ █ █▄░█ █▀▀
// ░█░ ██▄ ▄█ ░█░ █ █░▀█ █▄█

#include<stdio.h>

int main()
{
    printf("TESTING: ng-mem ");

    uint8_t data[100];
    uint8_t data2[200];


    // create first segment
    assert(mem_segments_amount==0);
    uint8_t segment_id_1 = ng_mem_segment_create(data,100);
    assert(ng_mem_segment_space_left(segment_id_1)==100);
    assert(mem_segments_amount==1);
    assert(segment_id_1==0);

    // create 2nd segment
    uint8_t segment_id_2 = ng_mem_segment_create(data2,200);
    assert(ng_mem_segment_space_left(segment_id_2)==200);
    assert(mem_segments_amount==2);
    assert(segment_id_2==1);

    // allocate block1 in 1st seg
    ng_mem_block_t block1={0};
    assert(block1.start_pos==0);
    assert(block1.size == 0);
    assert(block1.data == NULL);
    bool success = ng_mem_allocate(segment_id_1,20,1,&block1);
    assert(success);
    assert(ng_memblock_get_size(&block1) == 32); // 16 aligned.
    assert(block1.data != NULL);
    *(block1.data)=95;
    assert(*(block1.data)==95);
    ng_memblock_wipe(&block1);
    assert(*(block1.data)==0);

    // allocate block2 in 1st seg
    ng_mem_block_t block2={0};
    assert(block2.start_pos==0);
    assert(block2.size == 0);
    assert(block2.data == NULL);
    success = ng_mem_allocate(segment_id_1,40,1,&block2);
    assert(success);
    assert(ng_memblock_get_size(&block2) == 48); // 8 aligned
    assert(block2.data != NULL);
    *(block2.data)=95;
    assert(*(block2.data)==95);
    ng_memblock_wipe(&block2);
    assert(*(block1.data)==0);

    *(block1.data)=95;
    assert(*(block1.data)==95);
    *(block2.data)=95;
    assert(*(block2.data)==95);

    uint32_t spaceleft = ng_mem_segment_space_left(segment_id_1);
        
    ng_mem_block_t block3;
    ng_mem_allocate(segment_id_1,spaceleft,2,&block3);

    assert(2==ng_memblock_get_usage(&block3));

    ng_mem_block_t block4;
    spaceleft = ng_mem_segment_space_left(segment_id_1);
    assert(0 == spaceleft && "Should be no space anymore!");

    ng_mem_segment_wipe(segment_id_1);
    assert(*(block1.data)==0);
    assert(*(block2.data)==0);

    printf("... OK!\n");
    
    return 0;
}

#endif