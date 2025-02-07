
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

// peeks'n'pokes
// mouse btn
volatile uint8_t* mbtn = NULL;
volatile uint16_t* ms_delta = NULL;


#define TICK_RATE (1000/30)
#define delay 120


#define KEY_SPACE    (1 << 7)

keyboard_mapping_t kbm={
    .keycodes = {
        HID_KEY_SPACE,
        0,
        0,
        0,
        0,
        0,
        0,
        0},
    .flags = KEYBMAP_FLAG_SCAN_KEY_PRESSED
};


gfx_sprite_animator_t anim4x1={
    .animation_amount=4,
    .animations={
        {
            .start_tile=0,
            .end_tile=3,
            .delay_ms=delay,
        }
    }
};

uint8_t spritebuffer;
uint8_t sprite_bird_anim;

gfx_pixelbuffer_t pixelbuffer = {
    .width=64,
    .height=80,
    .x=0,
    .y=0,
    .pixel_size=flags_pack_4_4(5,3),
    //.flags=PXB_WRAPMODE(0,PXB_WRAPMODE_WRAP)
};

// 1 bird and two columns with 2 pillars each
#define PILLAR_AMOUNT 4
#define SPRITE_AMOUNT (1 + PILLAR_AMOUNT)
gfx_sprite_t sprites[SPRITE_AMOUNT];

gfx_sprite_t* sprite_bird=&sprites[0];
gfx_sprite_t* sprite_pillars[PILLAR_AMOUNT]={
    &sprites[1],
    &sprites[2],
    &sprites[3],
    &sprites[4]
};

gfx_tilesheet_data_t ts_bird,ts_pillar,ts_bg;

int8_t anim=0;

int seed = 13;

int random() {
    seed = seed * 1103515245 + 12345;
    return (unsigned int)(seed / 65536) % 32768;
}

int16_t x=0;

void init_system();
void init_gfx();
void init_audio();

int mod_init(){
    init_system();
    init_gfx();
    init_audio();
}

uint16_t last_x =0;
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

    // *(tile_map)=COL_RED;
    // *(tile_map+39)=COL_GREEN;
    // *(tile_map+39+29*40)=COL_ORANGE;
    // *(tile_map+29*40)=COL_VIOLETTE;
    

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
    // flags_set(sprite_oldguy->flags,SPRITEFLAG_DIRTY | SPRITEFLAG_FLIP_H);

    char text_bf[40];
    ng_snprintf(text_bf,40,"M-BTN:%d Key:%d",*mbtn,kbm.key_pressed);
    gfx_draw_text(4,2,text_bf,COL_ORANGE);


    kbm.key_down=0;
    kbm.key_pressed=0;
}

void init_gfx_bird();
void init_gfx_pillars();
void init_gfx_background();

void init_gfx() {
    // font
    gfx_set_font_from_asset(ASSET_FONT8);
    gfx_set_palette_from_assset(ASSET_COLOR_PALETTE,0);

    // sprint & bg
    spritebuffer = gfx_spritebuffer_create(sprites,SPRITE_AMOUNT);

    gfx_pixelbuffer_create(&pixelbuffer);
    gfx_pixelbuffer_set_active(&pixelbuffer);

    asset_get_tilesheet(&ts_bird,ASSET_SPRITE_BIRD);
    asset_get_tilesheet(&ts_pillar,ASSET_SPRITE_PILLAR);
    asset_get_tilesheet(&ts_bg,ASSET_BITMAP_BACKGROUND);
    
    gfx_tilesheet_current_set(&ts_bg);

    init_gfx_bird();
    init_gfx_pillars();
    init_gfx_background();

    // render objects and order! 
    gfx_renderqueue_add_id(pixelbuffer.obj_id);
    gfx_renderqueue_add_id(spritebuffer);
    gfx_renderqueue_apply();
}

void init_gfx_bird(){
    gfx_sprite_set_tileset(sprite_bird,&ts_bird,0);
    sprite_bird_anim = gfx_sprite_add_animator(sprite_bird,&anim4x1);
    gfx_spriteanimator_set_animation(sprite_bird_anim, 0, /*ANIMATIONFLAG_STOPPED |*/ ANIMATIONFLAG_LOOP);
    sprite_bird->x=120;
    sprite_bird->y=120;
    sprite_bird->flags = SPRITEFLAG_ALIGNH_CENTER | SPRITEFLAG_ALIGNV_CENTER;
    gfx_sprite_apply_data(sprite_bird);
}

void init_gfx_pillars(){
    for (uint8_t i=0; i < PILLAR_AMOUNT; i++){
        gfx_sprite_t* pillar = sprite_pillars[i];
        gfx_sprite_set_tileset(pillar,&ts_pillar,0);
        pillar->x=i * 40;
        pillar->y=10;
        pillar->pixel_size=flags_pack_4_4(1,2);
        gfx_sprite_apply_data(pillar);
    }
}

void init_gfx_background() {
    gfx_tilesheet_current_set(&ts_bg);
    gfx_draw_tile(0,0,0);
}

void init_system(){
    // TODO: assignment at declaration?
    ms_delta = (uint16_t*)MEMPTR(MM_MS_DELTA);
    mbtn = (uint8_t*)MEMPTR(MM_MOUSE_BTN);

    io_keyboardmapping_register(&kbm,1);
}

void init_audio(void){
    audio_mod_play(ASSET_GAME);
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