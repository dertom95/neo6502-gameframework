
#include <stdint.h>


#ifdef _MOD_NATIVE_
# include "../../source/core/memory.h"
# include "../../source/ng_all.h"
#else
# include <ng_config.h>
# include <gen/ng_api.c>
#endif

#include "export/assets.h"

volatile uint16_t* mx = NULL;
volatile uint16_t* my = NULL;
volatile uint8_t* mbtn = NULL;

//volatile uint8_t* tile_map = (uint8_t*)(0xd000);

volatile uint8_t* tile_map;

//volatile uint8_t* tile_map = (uint8_t*)(0x5000);
uint8_t col = 0;
uint8_t current_back=0;
uint8_t current_x=0;
uint8_t current_y=0;

#define KEY_COL_DOWN    (1 << 7)
#define KEY_COL_UP      (1 << 6)
#define KEY_LEFT    (1 << 5)
#define KEY_LEFT2      (1 << 4)
#define KEY_RIGHT2   (1 << 3)
#define KEY_RIGHT    (1 << 2)

keyboard_mapping_t kbm={
    .keycodes = {HID_KEY_0,HID_KEY_1,HID_KEY_A,HID_KEY_Q,HID_KEY_E,HID_KEY_D,0,0},
    .flags = KEYBMAP_FLAG_SCAN_KEY_PRESSED | KEYBMAP_FLAG_SCAN_KEY_DOWN
};

gfx_pixelbuffer_t pixelbuffer = {
    .width=40,
    .height=30,
    .x=0,
    .y=0,
    .canvas_x=2,
    .canvas_y=0,
    .canvas_width=140,
    .canvas_height=140,
    .pixel_size=flags_pack_4_4(8,8)
};

uint8_t kX=0,kY=0;

#define TICK_RATE (1000/30)
volatile uint16_t* ms_delta = NULL;

uint8_t px_width;
uint8_t px_height;


int mod_init(){
    ms_delta = (uint16_t*)MEMPTR(MM_MS_DELTA);
    mx =  (uint16_t*)MEMPTR(MM_MOUSE_Y);
    my =  (uint16_t*)MEMPTR(MM_MOUSE_Y);
    mbtn = (uint8_t*)MEMPTR(MM_MOUSE_BTN);
    tile_map = (uint8_t*)MEMPTR(0x5000);

    io_keyboardmapping_register(&kbm,1);


    // while(1){
    //     *(tile_map)=1;
    // }


    gfx_set_font_from_asset(ASSET_FONT8);
    gfx_set_palette_from_assset(ASSET_COLOR_PALETTE,0);

    gfx_pixelbuffer_create(&pixelbuffer);
    gfx_pixelbuffer_set_active(&pixelbuffer);

    gfx_renderqueue_add_id(pixelbuffer.obj_id);
    gfx_renderqueue_apply();


    gfx_pixelbuffer_mount(&pixelbuffer,0x5000);
#ifdef _MOD_NATIVE_    
    // memory mappings needs to be queried after they are mounted in _MOD_NATIVE_
    tile_map = (uint8_t*)MEMPTR(0x5000);
#endif

    flags_unpack_4_4(pixelbuffer.pixel_size,px_width,px_height);
}

void mod_update() {
    // TODO: implement some kind of sleep
    
    
    if (*ms_delta<TICK_RATE)
    {
        return;
    }
    *ms_delta=0;



    static uint8_t seed = 0;
    seed++;
    for (uint8_t i=0;i<40;i++){
        for (uint8_t j=0;j<30;j++){
            //  gfx_draw_pixel(i,j,seed+(i+1)*(j+1)); 
            *(tile_map+j*40+i)=seed+(i+1)*(j+1);
        }
    }

    bool changed = false;

    if ((kbm.key_down & KEY_LEFT)>0){
        pixelbuffer.x--;
        changed=true;
    }
    if ((kbm.key_down & KEY_RIGHT)>0){
        pixelbuffer.x++;
        changed=true;
    }
    if ((kbm.key_down & KEY_LEFT2)>0){
        pixelbuffer.canvas_x--;
        changed=true;
    }
    if ((kbm.key_down & KEY_RIGHT2)>0){
        pixelbuffer.canvas_x++;
        changed=true;
    }
    kbm.key_down=0;
    kbm.key_pressed=0;

    if (changed){
        pixelbuffer.pixel_size=flags_pack_4_4(px_width,px_height);
        gfx_pixelbuffer_set_active(&pixelbuffer);            
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