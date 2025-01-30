#include <stdint.h>


#include "ng_assets.h"
#include "ng_cpu.h"
#include <assert.h>
#include "core/roms.h"
#include "ng_gfx.h"
#include <string.h>


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
        assert(current_assetpack.offsets[i]==current_assetpack.offsets[i]);    
     //   assert(current_assetpack.sizes[i]==assets_sizes[i]);    
    }

    const uint8_t* code_data = assets_get_pointer(0);
    const uint16_t* start_address = (const uint16_t*)code_data;
    loadROM(code_data+2,(*start_address),current_assetpack.sizes[0]-2);
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

const uint32_t assets_get_size(uint8_t asset_id) {
    assert(current_assetpack.asset_amount>0 && "no asset pack set! set it with assets_set_current_pack");
    assert(asset_id < current_assetpack.asset_amount && "exceeded assets amount");
    const uint32_t result = current_assetpack.sizes[asset_id];
    return result;
}

// ---------------------------
// 🇦​​​​​🇸​​​​​🇸​​​​​🇪​​​​​🇹​​​​​ 🇱​​​​​🇴​​​​​🇦​​​​​🇩​​​​​🇮​​​​​🇳​​​​​🇬​​​​​
// ---------------------------

void asset_get_tilesheet(gfx_tilesheet_data_t* tilesheet_data,uint8_t asset_id){
	const gfx_tilesheet_t* assetdata = assets_get_pointer(asset_id);
	assert(flags_mask_value_is(assetdata->data.type,ASSET_TYPE_MASK,ASSET_TYPE_TILESHEET) && "Tried to get wrong asset-type!");

	gfx_tilesheet_t* tilesheet = ng_mem_allocate(SEGMENT_GFX_DATA,sizeof(gfx_tilesheet_t));
	
	//*tilesheet = *assetdata;
	memcpy(tilesheet,assetdata,sizeof(gfx_tilesheet_t));

	uint32_t cached_tileptr_size = sizeof(uint8_t*)*tilesheet->data.tile_amount;
	tilesheet->cached_tile_ptrs = ng_mem_allocate(SEGMENT_GFX_DATA,cached_tileptr_size);
	memset(tilesheet->cached_tile_ptrs,0,cached_tileptr_size);

	tilesheet->tilesheet_data_raw = ((uint8_t*)assetdata) + GFX_TILESHEETDATA_T_HEADER_SIZE;

    tilesheet->data.ts_id = id_store(tilesheet);
    *tilesheet_data = tilesheet->data;
}

void asset_get_tilemap(gfx_tilemap_t* tilemap,uint8_t asset_id){
    const gfx_tilemap_t* flashdata = assets_get_pointer(asset_id);
    memcpy(tilemap,flashdata,GFX_TILEMAP_T_HEADER_SIZE);

    gfx_internal_tilemap_t* internal_tilemap = ng_mem_allocate(SEGMENT_GFX_DATA,sizeof(gfx_internal_tilemap_t));
    *internal_tilemap = (gfx_internal_tilemap_t){0};
    
    ng_mem_allocate_block(SEGMENT_GFX_DATA,sizeof(gfx_tilemap_layer_t*)*tilemap->layer_amount,MEM_USAGE_TILEMAP,&internal_tilemap->mem);
    internal_tilemap->tilemap_data=tilemap;
    internal_tilemap->layers=(gfx_tilemap_layer_t**)internal_tilemap->mem.data;
    internal_tilemap->rawdata = (void*)flashdata;
    ng_memblock_wipe(&internal_tilemap->mem);
    tilemap->handle = id_store(internal_tilemap);
}
