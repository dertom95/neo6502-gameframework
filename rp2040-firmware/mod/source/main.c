
#include <stdint.h>
#include <ng_config.h>
#include <gen/ng_api.c>

#include "export/assets.h"

volatile uint16_t* mx =  (uint16_t*)MM_MOUSE_X;
volatile uint16_t* my =  (uint16_t*)MM_MOUSE_Y;
volatile uint8_t* mbtn = (uint8_t*)MM_MOUSE_BTN;

volatile uint8_t* tile_map = (uint8_t*)MEMORY_TILEAREA_BEGIN;
uint8_t col = 0;
uint8_t current_back=0;
uint8_t current_x=0;
uint8_t current_y=0;

#define KEY_COL_DOWN (1 << 7)
#define KEY_COL_UP   (1 << 6)

keyboard_mapping_t kbm[]={
    {
        .keycodes = {HID_KEY_0,HID_KEY_1,0,0,0,0,0,0},
        .flags = KEYBMAP_FLAG_SCAN_KEY_PRESSED
    }
};

gfx_pixelbuffer_t pixelbuffer = {
    .width=100,
    .height=100,
    .x=1,
    .y=2    
};

uint8_t kX=0,kY=0;

#define TICK_RATE (1000/30)
uint16_t* ms_delta = (uint16_t*)MM_MS_DELTA;

int main(){
    io_keyboardmapping_register(kbm,1);


    // while(1){
    //     *(tile_map)=1;
    // }


    gfx_set_font_from_asset(ASSET_FONT8);
    gfx_set_palette_from_assset(ASSET_COLOR_PALETTE,0);

    gfx_pixelbuffer_create(&pixelbuffer);
    gfx_pixelbuffer_set_active(&pixelbuffer);

    gfx_renderqueue_add_id(pixelbuffer.obj_id);
    gfx_renderqueue_apply();
    while(1){
        // TODO: implement some kind of sleep
        if (*ms_delta<TICK_RATE)
        {
            continue;
        }
        *ms_delta=0;

        uint8_t x = *mx / 8;
        uint8_t y = *my / 8;

        if ((kbm[0].key_pressed & KEY_COL_DOWN)>0){
            col--;
        }
        if ((kbm[0].key_pressed & KEY_COL_UP)>0){
            col++;
        }

        if (current_x!=x || current_y!=y){
            *(tile_map+current_y*40+current_x)=current_back;        
            
            current_back = *(tile_map+y*40+x);
            //current_back = COL_GREEN;
            current_x=x;
            current_y=y;
        }
        {
            uint8_t btn_left = *mbtn;
            if ((btn_left&MOUSE_BUTTON_LEFT)>0){
                *(tile_map+y*40+x)=COL_RED;
                current_back=COL_RED;        
            } else {
                *(tile_map+y*40+x)=col;        
            }
        }        

    }
    // asset_set_current_pack
    return 0;
}