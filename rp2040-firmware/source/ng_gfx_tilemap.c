#include "ng_gfx.h"



#ifdef PICO_NEO6502
# include "core/backend/neo6502/neo6502.h"
//#include "pico/platform.h"
#include "boards/pico.h"
#endif
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

extern uint16_t color_palette[];
//#include "zelda_mini_plus_walk_rgab5515.h"
#include <string.h>

#include "api/gen/ng_api.h"

#include "ng_io.h"
#include <math.h>

#include "ng_utils.h"
#include <assert.h>
#include "core/memory.h"
#include "ng_assets.h"
#include "ng_utils.h"

void gfx_load_tilemap_layer(gfx_tilemap_t* tm,gfx_tilemap_layer_t* out_layer, uint8_t layer_nr)
{
    assert(layer_nr < tm->layer_amount   && "layer_nr outofbounds");
    gfx_internal_tilemap_t* tm_internal = id_get_ptr(tm->handle);
    assert(tm_internal->layers[layer_nr]==NULL && "layer already loaded!");
    tm_internal->layers[layer_nr]=out_layer;
    out_layer->tm_handle=tm->handle;
    
    //TODO: this is a bit chaotic to get the data I want! :D 
    uint8_t* tip = tm_internal->rawdata;
    tip+=4;
    uint16_t* offsets = (uint16_t*)tip;
    offsets+=layer_nr;
    uint16_t offset = swap16(*offsets);  // read offset to layerdata and handle endianess if needed
    uint16_t amount_tiles = tm->tileamount_horizontal * tm->tileamount_vertical;
    
    tip = tm_internal->rawdata;
    tip += offset;
    memcpy(&out_layer->layer_id,tip,GFX_TILEMAP_LAYER_T_HEADER_SIZE+amount_tiles);
    tip +=GFX_TILEMAP_LAYER_T_HEADER_SIZE+amount_tiles;
    assert(*tip==95);
}