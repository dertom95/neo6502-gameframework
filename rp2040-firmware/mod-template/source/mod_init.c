
#include <stdint.h>
#include <stdio.h>


#ifdef _MOD_NATIVE_
# include "../../source/core/memory.h"
# include "../../source/ng_all.h"
#else
# include <ng_config.h>
# include "../../source/ng_all.h"
#endif

#include "export/assets.h"
#include <ng_api_shared.h>

#define PIXEL_SIZE 2
#define TICKRATE (1000/30)
// mapped to address
volatile uint16_t* ms_delta = NULL;

gfx_pixelbuffer_t pixelbuffer = {
    .width=320/PIXEL_SIZE,
    .height=240/PIXEL_SIZE,
    .pixel_size=flags_pack_4_4(PIXEL_SIZE,PIXEL_SIZE)
};

uint8_t current_color_index = 0;

// █ █▄░█ █ ▀█▀
// █ █░▀█ █ ░█░

int mod_init(){
    ms_delta = (uint16_t*)MEMPTR(MM_MS_DELTA);
    // font
    gfx_set_font_from_asset(ASSET_FONT8);
    gfx_set_palette_from_assset(ASSET_COLOR_PALETTE,0);    

    // init goes here
    gfx_pixelbuffer_create(&pixelbuffer);
    gfx_pixelbuffer_set_active(&pixelbuffer);
    gfx_draw_text(00,10,"Hello World!",COL_BLACK,COL_WHITE);

    // put the pixelbuffer in the render-queue to be rendered at all
    gfx_renderqueue_add_id(pixelbuffer.obj_id);
    gfx_renderqueue_apply();
}


// █░█ █▀█ █▀▄ ▄▀█ ▀█▀ █▀▀P
// █▄█ █▀▀ █▄▀ █▀█ ░█░ ██▄

void mod_update() {
    if (*ms_delta<TICK_RATE)
    {
        return;
    }
    *ms_delta = 0;

    // the tick goes here!
    // TODO: cached keyboard check
    if (io_keyboard_is_pressed(HID_KEY_SPACE)){
        current_color_index++;
        gfx_draw_text(10,10,"Hello World!",current_color_index,COL_WHITE);
    }
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