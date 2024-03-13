
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
    .keycodes = {
        HID_KEY_0,
        HID_KEY_1,
        HID_KEY_A,
        HID_KEY_Q,
        HID_KEY_E,
        HID_KEY_D,
        0,
        0},
    .flags = KEYBMAP_FLAG_SCAN_KEY_PRESSED | KEYBMAP_FLAG_SCAN_KEY_DOWN
};

gfx_pixelbuffer_t pixelbuffer = {
    .width=320,
    .height=240,
    .x=0,
    .y=0,
    .pixel_size=flags_pack_4_4(1,1),
    //.flags=PXB_WRAPMODE(0,PXB_WRAPMODE_WRAP)
};

gfx_sprite_animator_t anim4x3={
    .animation_amount=4,
    .animations={
        {
            .start_tile=0,
            .end_tile=2,
            .delay_ms=125,
        },
        {
            .start_tile=3,
            .end_tile=4,
            .delay_ms=125,
        },
        {
            .start_tile=6,
            .end_tile=8,
            .delay_ms=125,
        },
        {
            .start_tile=9,
            .end_tile=10,
            .delay_ms=125,
        },
    }
};

gfx_sprite_animator_t anim4={
    .animation_amount=1,
    .animations={
        {
            .start_tile=4,
            .end_tile=6,
            .delay_ms=125
        }
    }
};


uint8_t spritebuffer;

uint8_t kX=0,kY=0;
char text_bf[30];

#define TICK_RATE (1000/30)
volatile uint16_t* ms_delta = NULL;

uint8_t px_width;
uint8_t px_height;

gfx_tilesheet_data_t ts_misc;
gfx_tilesheet_data_t ts_oldguy;


#define SPRITE_AMOUNT 32
gfx_sprite_t sprites[SPRITE_AMOUNT];

gfx_sprite_t* sprite_oldguy=&sprites[0];
gfx_sprite_t* sprite_strawberry=&sprites[1];
gfx_sprite_t* sprite_potion=&sprites[2];

uint8_t sprite_potion_anim;
uint8_t sprite_oldguy_anim;
int8_t anim=0;

int seed = 13;

int random() {
    seed = seed * 1103515245 + 12345;
    return (unsigned int)(seed / 65536) % 32768;
}



int mod_init(){
    ms_delta = (uint16_t*)MEMPTR(MM_MS_DELTA);
    mx =  (uint16_t*)MEMPTR(MM_MOUSE_X);
    my =  (uint16_t*)MEMPTR(MM_MOUSE_Y);
    mbtn = (uint8_t*)MEMPTR(MM_MOUSE_BTN);
    tile_map = (uint8_t*)MEMPTR(0x5000);

    asset_get_tilesheet(&ts_oldguy,ASSET_OLD_GUY);
    asset_get_tilesheet(&ts_misc,ASSET_SPRITES_MISC);

    io_keyboardmapping_register(&kbm,1);

    // while(1){
    //     *(tile_map)=1;
    // }

    gfx_set_font_from_asset(ASSET_FONT8);
    gfx_set_palette_from_assset(ASSET_COLOR_PALETTE,0);

    gfx_pixelbuffer_create(&pixelbuffer);
    gfx_pixelbuffer_set_active(&pixelbuffer);

    gfx_pixelbuffer_mount(&pixelbuffer,0x5000);
#ifdef _MOD_NATIVE_    
    // memory mappings needs to be queried after they are mounted in _MOD_NATIVE_
    tile_map = (uint8_t*)MEMPTR(0x5000);
#endif

    spritebuffer = gfx_spritebuffer_create(sprites,SPRITE_AMOUNT);

    gfx_sprite_set_tileset(sprite_oldguy,&ts_oldguy,0);
    flags_set(sprite_oldguy->flags,SPRITEFLAG_FLIP_H);

    sprite_oldguy_anim = gfx_sprite_add_animator(sprite_oldguy,&anim4x3);
  //  flags_set(sprite_oldguy->flags, SPRITEFLAG_FLIP_H | SPRITEFLAG_FLIP_V | SPRITEFLAG_ALIGNH_CENTER | SPRITEFLAG_ALIGNV_CENTER);
    
    gfx_sprite_set_tileset(sprite_strawberry,&ts_misc,3);
    flags_set(sprite_strawberry->flags,SPRITEFLAG_FLIP_V);
    
    gfx_sprite_set_tileset(sprite_potion,&ts_misc,4);
    sprite_potion_anim = gfx_sprite_add_animator(sprite_potion,&anim4);
    gfx_spriteanimator_set_animation(sprite_potion_anim, 0, ANIMATIONFLAG_BACKWARDS | ANIMATIONFLAG_LOOP);

    sprite_oldguy->x=50;
    sprite_oldguy->y=50;
    sprite_oldguy->pixel_size=flags_pack_4_4(1,1);

    sprite_strawberry->x=50;
    sprite_strawberry->y=40;

    for (int i=3;i<SPRITE_AMOUNT;i++){
        gfx_sprite_t* spr = &sprites[i];
        uint8_t tile_id = i % 5;
        gfx_sprite_set_tileset(spr,&ts_misc,tile_id);
        if (tile_id==4){
            sprite_potion_anim = gfx_sprite_add_animator(spr,&anim4);
            gfx_spriteanimator_set_animation(sprite_potion_anim, 0, ANIMATIONFLAG_BACKWARDS | ANIMATIONFLAG_LOOP);
        }
        spr->x=(i*40) % 300;
        spr->y=50+((i*40) / 300)*40;
        //spr->pixel_size=flags_pack_4_4((random()%2+1),(random()%2+1));
        spr->pixel_size=flags_pack_4_4(i%2+1,i%2+1);
    }

    sprite_potion->x=70;
    sprite_potion->y=40;

    // sprite_strawberry->flags=0;
    // sprite_potion->flags=0;

    gfx_renderqueue_add_id(pixelbuffer.obj_id);
   // gfx_renderqueue_add_id(spritebuffer);

    gfx_renderqueue_apply();


    flags_unpack_4_4(pixelbuffer.pixel_size,px_width,px_height);
}


void mod_update() {
    // TODO: implement some kind of sleep
    
    
    if (*ms_delta<TICK_RATE)
    {
        return;
    }

    int16_t dt = *ms_delta;
    gfx_spritebuffer_update(dt,spritebuffer);
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

    // pixelbuffer.x = *mx-((pixelbuffer.width*px_width)/2);
    // pixelbuffer.y = *my-((pixelbuffer.height*px_height)/2);
    //gfx_sprite_set_position(sprite_oldguy,*mx-ts_data.tile_width/2,*my-ts_data.tile_height/2);
    // sprite_oldguy->x=*mx;
    // sprite_oldguy->y=*my;

    
    pixelbuffer.y=*my;

    ng_snprintf(text_bf,30,"M %d : %d",*mx,*my);
    gfx_draw_text(4,2,text_bf,COL_ORANGE);

    if ((kbm.key_pressed & KEY_LEFT)>0){
        if (sprite_oldguy->tile_idx-1 >= 0){
            gfx_sprite_set_tileid(sprite_oldguy, sprite_oldguy->tile_idx-1);
        }
        changed=true;
    }
    if ((kbm.key_pressed & KEY_RIGHT)>0){
        if (sprite_oldguy->tile_idx+1 < ts_oldguy.tile_amount){
            gfx_sprite_set_tileid(sprite_oldguy, sprite_oldguy->tile_idx+1);
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

    // if (flags_isset(kbm.key_pressed,KEY_COL_UP)){
    //     anim++;
    //     if (anim>3){
    //         anim=0;
    //     }
    //     gfx_spriteanimator_set_animation(sprite_oldguy_anim,anim,ANIMATIONFLAG_LOOP);
    // }
    // else if (flags_isset(kbm.key_pressed,KEY_COL_DOWN)){
    //     anim--;
    //     if (anim<0){
    //         anim=3;
    //     }
    //     gfx_spriteanimator_set_animation(sprite_oldguy_anim,anim,ANIMATIONFLAG_LOOP);
    // }

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