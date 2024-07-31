
#include <stdint.h>
#include <stdio.h>


#ifdef _MOD_NATIVE_
# include "../../source/core/memory.h"
# include "../../source/ng_all.h"
#else
# include <ng_config.h>
# include <gen/ng_api.c>
#endif

#include "export/assets.h"
#include <ng_api_shared.h>

volatile uint16_t* mx = NULL;
volatile uint16_t* my = NULL;
volatile uint8_t* mbtn = NULL;
volatile uint8_t* pxbuf = NULL;

#define SCREEN_WIDTH 160
#define SCREEN_HEIGHT 120
#define PX (320/SCREEN_WIDTH)
#define PY (240/SCREEN_HEIGHT)
#define TICK_RATE (1000/30)
volatile uint16_t* ms_delta = NULL;

uint8_t px_width;
uint8_t px_height;


int16_t x=0;


// tilemap
gfx_tilesheet_data_t ts_spritesheet;
gfx_tilemap_t tilemap = {0};

typedef struct gfx_layer_30_30_t {
    gfx_tilemap_layer_t layer_data;
    uint8_t data[30*30];
} gfx_layer_30_30_t;

gfx_layer_30_30_t layer0 = {0};


// pixelbuffer
gfx_pixelbuffer_t pixelbuffer = {
    .width=SCREEN_WIDTH,
    .height=SCREEN_HEIGHT,
    .x=0,
    .y=0,
    .pixel_size=flags_pack_4_4(PX,PY),
    //.flags=PXB_WRAPMODE(0,PXB_WRAPMODE_WRAP)
};





int mod_init(){
    // tilemap
    asset_get_tilesheet(&ts_spritesheet,ASSET_SPRITESHEET);

    asset_get_tilemap(&tilemap,ASSET_TILEMAP);
    tilemap.tilesheet_id=ts_spritesheet.ts_id;
    gfx_load_tilemap_layer(&tilemap,&layer0,0);



    ms_delta = (uint16_t*)MEMPTR(MM_MS_DELTA);
    mx =  (uint16_t*)MEMPTR(MM_MOUSE_X);
    my =  (uint16_t*)MEMPTR(MM_MOUSE_Y);
    mbtn = (uint8_t*)MEMPTR(MM_MOUSE_BTN);

    gfx_set_font_from_asset(ASSET_FONT8);
    gfx_set_palette_from_assset(ASSET_COLOR_PALETTE,0);

    gfx_set_font_from_asset(ASSET_FONT8);
    gfx_set_palette_from_assset(ASSET_COLOR_PALETTE,0);

    gfx_pixelbuffer_create(&pixelbuffer);
    gfx_pixelbuffer_set_active(&pixelbuffer);

    gfx_draw_pixel(0,0,COL_RED);
    gfx_draw_pixel(SCREEN_WIDTH-1,0,COL_GREEN);
    gfx_draw_pixel(SCREEN_WIDTH-1,SCREEN_HEIGHT-1,COL_GREEN);
    gfx_draw_pixel(0,SCREEN_HEIGHT-1,COL_RED);

    gfx_draw_tilemap_layer(10,10,&layer0);

    gfx_renderqueue_add_id(pixelbuffer.obj_id);

    gfx_renderqueue_apply();

    flags_unpack_4_4(pixelbuffer.pixel_size,px_width,px_height);
    int a=0;
}

uint16_t last_x =0;

void mod_update() {
    // TODO: implement some kind of sleep
    if (*ms_delta<TICK_RATE)
    {
        return;
    }

    last_x++;
    if (last_x==240){
        last_x=0;
    }
    
    int16_t dt = *ms_delta;
    *ms_delta=0;

    bool changed = false;

   
    //flags_set(pixelbuffer.flags,PXBFLAG_DIRTY);
}

#ifndef _MOD_NATIVE_
int main(){
    mod_init();

    while(1){
        mod_update();
    }

    return 0;
}
#endif