#include <stdint.h>


#include "ng_assets.h"

assetpack_t* current_assetpack = NULL;

// get void pointer for an asset_id
void assets_set_current_pack(assetpack_t* asset_pack )
{
    current_assetpack = asset_pack;
}

assetpack_t* assets_get_current_pack(void)
{
    return current_assetpack;
}

const void*  assets_get_pointer(uint8_t asset_id)
{
    assert(current_assetpack!=NULL && "no asset pack set! set it with assets_set_current_pack");
    assert(asset_id < current_assetpack->asset_amount && "exceeded assets amount");
    const void* tip = current_assetpack->data+current_assetpack->offsets[asset_id];
    return tip;
}
