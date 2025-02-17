
#include <stdint.h>
#include <stdio.h>

#include "tests.h"

volatile uint16_t* ms_delta = NULL;
volatile uint8_t* ms_scanline = NULL;

uint8_t current_color_index = 0;

#define INSTRUCTION_LINES 2
gfx_pixelbuffer_t pxb_instructions = {
    .y=240-(INSTRUCTION_LINES*8),
    .width=320,
    .height=(INSTRUCTION_LINES*8),
    .pixel_size=flags_pack_4_4(1,1)
};

// █ █▄░█ █ ▀█▀
// █ █░▀█ █ ░█░

void draw_instructions(const char* instructions){
    ng_debug_value(0xf0,1);
    gfx_pixelbuffer_t* current_pxb = gfx_pixelbuffer_get_current();
    gfx_pixelbuffer_wipe(&pxb_instructions,COL_TRANSPARENT);

    ng_debug_value(0xf0,2);
    gfx_pixelbuffer_set_active(&pxb_instructions);
    gfx_draw_text(0,0,instructions, COL_BLACK, COL_GREY_5);
    ng_debug_value(0xf0,3);
    gfx_pixelbuffer_set_active(current_pxb);
}

int mod_init(){
//     ms_delta = (uint16_t*)MEMPTR(MM_MS_DELTA);
    ms_scanline = (uint8_t*)MEMPTR(MM_SCANLINE);
//     // font
    gfx_set_font_from_asset(ASSET_FONT8);
    gfx_set_palette_from_assset(ASSET_COLOR_PALETTE,0);    

    gfx_pixelbuffer_create(&pxb_instructions);
    test1_init();

//     // init goes here
//     gfx_pixelbuffer_create(&pixelbuffer);
//     gfx_pixelbuffer_set_active(&pixelbuffer);
//     gfx_draw_text(0,0,"Hello World!",COL_BLACK,COL_WHITE);
// //    gfx_draw_button(&buttons[0]);
//     // buttons[0].flags=GFX_BTNSTATE_PRESSED;
//     // buttons[0].y+=10;
    
//     // gfx_draw_button(&buttons[0]);

//     // put the pixelbuffer in the render-queue to be rendered at all
//     gfx_renderqueue_add_id(pixelbuffer.obj_id);
//     gfx_renderqueue_apply();

}


// █░█ █▀█ █▀▄ ▄▀█ ▀█▀ █▀▀P
// █▄█ █▀▀ █▄▀ █▀█ ░█░ ██▄


typedef enum State {
    state_wait_for_scan_end, state_wait_for_start
} State;

State state = state_wait_for_start;


bool wait_for_end() {
    return *ms_scanline >= 239;
}


void mod_update() {
    if (!wait_for_end()){
        return;
    }

    test1_update();
    // // the tick goes here!
    // if (io_keyboard_is_pressed(HID_KEY_SPACE)){
    //     current_color_index++;
    //     gfx_draw_text(10,10,"Hello World!",current_color_index,COL_WHITE);
    // }

 //   gfx_ui_update();
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