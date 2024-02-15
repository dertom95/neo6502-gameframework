#ifndef __NG_ASSETS_H__
#define __NG_ASSETS_H__

#include "ng_defines.h"

#include "../mod/export/assets.h"

typedef struct assetpack_t {
    uint8_t asset_amount;
    const uint8_t* data;
    const uint32_t* sizes;
    const uint32_t* offsets;
} assetpack_t;



// get void pointer for an asset_id
void            assets_set_current_pack(assetpack_t* mgr );
assetpack_t*    assets_get_current_pack(void);
const uint8_t*           assets_get_pointer(uint8_t asset_id);

#endif