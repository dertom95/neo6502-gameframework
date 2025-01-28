#ifndef __NG_ASSETS_H__
#define __NG_ASSETS_H__

#include "ng_defines.h"

#include "../mod/export/assets.h"
#include "api/ng_api_shared.h"

typedef struct assetpack_t {
    uint8_t asset_amount;
    const uint8_t* data;
    const uint32_t* sizes;
    const uint32_t* offsets;
} assetpack_t;

// get void pointer for an asset_id
void            assets_set_current_pack(void* mgr, uint32_t size);
assetpack_t*    assets_get_current_pack(void);
const void*     assets_get_pointer(uint8_t asset_id);

/*api:4:1*/void asset_get_tilesheet(gfx_tilesheet_data_t* ts_data,uint8_t asset_id);
/*api:4:2*/void asset_get_tilemap(gfx_tilemap_t* tilemap,uint8_t asset_id);

#endif