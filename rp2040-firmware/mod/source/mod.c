
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
    .width=100,
    .height=20,
    .x=-2,
    .y=0,
    .pixel_size=flags_pack_4_4(1,1),
    .flags=PXB_WRAPMODE(0,PXB_WRAPMODE_WRAP)
};

uint8_t kX=0,kY=0;
char text_bf[30];

#define TICK_RATE (1000/30)
volatile uint16_t* ms_delta = NULL;

uint8_t px_width;
uint8_t px_height;


int mod_init(){
    ms_delta = (uint16_t*)MEMPTR(MM_MS_DELTA);
    mx =  (uint16_t*)MEMPTR(MM_MOUSE_X);
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

    *(tile_map)=COL_RED;
    *(tile_map+39)=COL_GREEN;
    *(tile_map+39+29*40)=COL_ORANGE;
    *(tile_map+29*40)=COL_VIOLETTE;
    

    // for (uint8_t i=0;i<4;i++){
    //     for (uint8_t j=0;j<4;j++){
    //         *(tile_map+j*40+i)=COL_RED;
    //         *(tile_map+j*40+i+20)=COL_GREEN;
    //         *(tile_map+(j+25)*40+i+25)=COL_ORANGE;
    //         *(tile_map+(j+20)*40+i)=COL_VIOLETTE;
    //     }
    // }

    bool changed = false;

    pixelbuffer.x = *mx-((pixelbuffer.width*px_width)/2);
    pixelbuffer.y = *my-((pixelbuffer.height*px_height)/2);

    ng_snprintf(text_bf,30,"M %d : %d",*mx,*my);

    gfx_draw_text(4,2,text_bf,COL_ORANGE);


    if ((kbm.key_pressed & KEY_LEFT)>0){
        if (px_width>0){
            px_width--;
        }
        changed=true;
    }
    if ((kbm.key_pressed & KEY_RIGHT)>0){
        if (px_width<15){
            px_width++;
        }
        changed=true;
    }
    if ((kbm.key_pressed & KEY_LEFT2)>0){
        // if (px_height>0){
        //     px_height--;
        // }
        pixelbuffer.x--;
        changed=true;
    }
    if ((kbm.key_pressed & KEY_RIGHT2)>0){
        // if (px_height<15){
        //     px_height++;
        // }
        pixelbuffer.x++;
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