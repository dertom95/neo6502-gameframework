#include <stdint.h>


#include "ng_assets.h"
#include "ng_cpu.h"
#include <assert.h>
#include "core/roms.h"


assetpack_t current_assetpack;

// get void pointer for an asset_id
void assets_set_current_pack(void* data, uint32_t size)
{
    current_assetpack = (assetpack_t){0};

    uint8_t* ptr = (uint8_t*)data;
    
    // read asset amount
    uint8_t amount = *ptr;
    current_assetpack.asset_amount = amount;

    // jump to the offset-file at the end o
    ptr += size - amount * 2 * 4; 
    current_assetpack.data = data;
    current_assetpack.offsets = (uint32_t*)ptr;

    ptr+= amount*4;
    current_assetpack.sizes = (uint32_t*)ptr;

    for (int i=0;i<amount;i++){
        assert(current_assetpack.offsets[i]==assets_offsets[i]);    
        assert(current_assetpack.sizes[i]==assets_sizes[i]);    
    }

    const uint8_t* code_data = assets_get_pointer(0);
    loadROM(code_data,0x1000,current_assetpack.sizes[0]);
    ng_cpu_start();
}

assetpack_t* assets_get_current_pack(void)
{
    return &current_assetpack;
}

const void*  assets_get_pointer(uint8_t asset_id)
{
    assert(current_assetpack.asset_amount>0 && "no asset pack set! set it with assets_set_current_pack");
    assert(asset_id < current_assetpack.asset_amount && "exceeded assets amount");
    const void* tip = current_assetpack.data+current_assetpack.offsets[asset_id];
    return tip;
}
