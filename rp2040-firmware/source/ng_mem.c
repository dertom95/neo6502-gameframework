#include "ng_mem.h"
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>

// the memory segments
uint8_t          mem_segments_amount=0;
ng_mem_segment_t mem_segments[MEM_MAX_SEGMENTS]={0};

uint8_t datamount_amount=0;
ng_mem_datamount_t* datamounts[MAX_DATAMOUNTS]={0};

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
    uint8_t segment_id = mem_segments_amount++;
    ng_mem_segment_wipe(segment_id);
    return segment_id;
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


void* ng_mem_allocate(uint8_t segment_id,uint32_t size)
{
    return ng_mem_allocate_with_alignment(segment_id,size,4);
}

void* ng_mem_allocate_with_alignment(uint8_t segment_id,uint32_t size,uint8_t alignment)
{
    assert(segment_id < mem_segments_amount && "unknown segment_id");
    assert(size < 131056 && "ng_allocte: exceeded size. size must not be >= 131056");

    ng_mem_segment_t* segment = &mem_segments[segment_id];
    
    uint8_t modulo = ((size_t)segment->tip)%alignment;
    
    if (modulo!=0){
        // not aligned. Add bytes to start-pos to align
        uint8_t add_bytes = alignment-modulo;
        segment->tip  += add_bytes;
        segment->used += add_bytes;
    }
    
    uint32_t space_left = segment->size - segment->used;

    if ( (size & 1) == 1){
        size+=1; // we are losing one byte storing our size in 16bit. Therefore we add one to the size to make it one bigger than we need
    }

    assert(space_left >= size && "exceeded segment-memory");
    
    void* result = segment->tip;
    
    segment->tip += size;
    segment->used += size;
    return result;
}


// allocates a block of memory in the specified segment. sizes needs to be multiple of 8. max-siz: 131056 and/or free space in segment
bool ng_mem_allocate_block(uint8_t segment_id,uint32_t size, uint8_t usage_type, ng_mem_block_t* block)
{
    assert(usage_type < 16 && "exceeded usage-type");
    assert(block!=NULL && "ng_allocate: you need to provide an output-block");

    void* data = ng_mem_allocate_with_alignment(segment_id,size,16);
    block->data = data;
    block->flags =  usage_type | (segment_id << 4);
//    block->size = size >> 1;
    block->size = size;

    return true;
}

uint8_t  ng_memblock_get_usage(ng_mem_block_t* mem_block)
{
    uint8_t result = (uint8_t)(mem_block->flags & MEMBLOCK_USAGE_MASK);
    return result;
}

uint8_t ng_mem_block_get_segmentid(ng_mem_block_t* block)
{
    uint8_t result = (uint8_t)((block->flags & MEMBLOCK_SEGMENT_MASK) >> 4);
    return result;
}



uint32_t ng_memblock_get_size(ng_mem_block_t* mem_block)
{
    uint32_t result = (mem_block->size << 1);
    return result;
}

// wipe the block with 0
void ng_memblock_wipe(ng_mem_block_t* mem_block)
{
    uint32_t size = ng_memblock_get_size(mem_block);
    memset(mem_block->data,0,size);
}




void ng_mem_add_datamount(ng_mem_datamount_t* data_mount)
{
    assert(datamount_amount<MAX_DATAMOUNTS&&"exceeded max amount of datamounts");
    datamounts[datamount_amount++]=data_mount;
}

void ng_mem_mount_block(ng_mem_block_t* data_block, uint16_t destination) {
    ng_mem_block_t save = *data_block;
    ng_mem_datamount_t* mount = ng_mem_allocate(SEGMENT_GFX_DATA,sizeof(ng_mem_datamount_t));
    mount->destination = destination;
    mount->size = save.size;
    mount->source = save.data;
    ng_mem_add_datamount(mount);
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
    assert(block1.size == 0);
    assert(block1.data == NULL);
    bool success = ng_mem_allocate_block(segment_id_1,20,1,&block1);
    assert(success);
    assert(ng_memblock_get_size(&block1) == 20);
    assert(block1.data != NULL);
    *(block1.data)=95;
    assert(*(block1.data)==95);
    ng_memblock_wipe(&block1);
    assert(*(block1.data)==0);

    // allocate block2 in 1st seg
    ng_mem_block_t block2={0};
    assert(block2.size == 0);
    assert(block2.data == NULL);
    success = ng_mem_allocate_block(segment_id_1,40,1,&block2);
    assert(success);
    assert(ng_memblock_get_size(&block2) == 40); 
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
    ng_mem_allocate_block(segment_id_1,spaceleft,2,&block3);

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