
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

#include "moorhuhn.h"

volatile uint8_t* mbtn = NULL;
volatile uint16_t* ms_delta = NULL;
volatile uint16_t* mouse_x = NULL;
volatile uint16_t* mouse_y = NULL;
volatile uint8_t* mouse_btn_state_pressed = NULL;




gfx_pixelbuffer_t pixelbuffer = {
    .width=320/PIXEL_SIZE,
    .height=240/PIXEL_SIZE,
    .pixel_size=flags_pack_4_4(PIXEL_SIZE,PIXEL_SIZE)
};

uint8_t current_color_index = 0;






mh_renderstate_t mh_rs={0};



void mh_gfx_init() {
    mh_rs.spritebuffer_id = gfx_spritebuffer_create(mh_rs.sprites,SPRITEAMOUNT);
    asset_get_tilesheet(&mh_rs.ts_bird,ASSET_SPRITE_BIRD_16);
    asset_get_tilesheet(&mh_rs.ts_crosshair_16,ASSET_SPRITE_CROSSHAIR_16);
    
    for (uint8_t i=0;i<INIT_MOORHUHN_AMOUNT;i++){
        mh_huhn_t huhn_data = {
            .x = utils_random_uint16() % 320,
            .y = utils_random_uint16() % 200,
            .velocity = (utils_random_uint16() % 2)==0 ? -1 : +1,
            .flags = MH_HUHNFLAG_ALIVE,
            .hitpoints = 1,
        };
        
        mh_huhn_t* huhn=NULL;
        bool spawn_successful = mh_huhn_spawn(&huhn_data, MH_HUHNTYPE_DEFAULT, &huhn);
        ASSERT_STRICT(spawn_successful);
        if (!spawn_successful){
            break;
        }
    }

    uint8_t sprite_id = gfx_spritebuffer_find_free_sprite(mh_rs.spritebuffer_id);
    bool found_sprite = sprite_id != 255;
    mh_rs.crosshair = &mh_rs.sprites[sprite_id];

    ASSERT_STRICT(found_sprite);

    gfx_sprite_set_tileset(mh_rs.crosshair,&mh_rs.ts_crosshair_16,0);
    mh_rs.crosshair->flags = SPRITEFLAG_ENABLED | SPRITEFLAG_IN_USE | SPRITEFLAG_ALIGNH_CENTER | SPRITEFLAG_ALIGNV_CENTER;
    mh_rs.crosshair->x = 160;
    mh_rs.crosshair->y = 120;
    uint8_t sprite_anim = gfx_sprite_add_animator(mh_rs.crosshair, &anim4x1_60);
    gfx_spriteanimator_set_animation(sprite_anim, 0, ANIMATIONFLAG_STOPPED);

    gfx_sprite_apply_data(mh_rs.crosshair);


    mh_update_huhn_positions();
}



// █ █▄░█ █ ▀█▀
// █ █░▀█ █ ░█░

int mod_init(){
    ms_delta = (uint16_t*)MEMPTR(MM_MS_DELTA);
    mbtn = (uint8_t*)MEMPTR(MM_MOUSE_BTN);
    mouse_btn_state_pressed = MEMPTR(MM_MOUSE_BTN_PRESSED);    
    mouse_x = (uint16_t*) MEMPTR(MM_MOUSE_X);
    mouse_y = (uint16_t*) MEMPTR(MM_MOUSE_Y);

    // font
    gfx_set_font_from_asset(ASSET_FONT8);
    gfx_set_palette_from_assset(ASSET_COLOR_PALETTE,0);    

    // init goes here
    gfx_pixelbuffer_create(&pixelbuffer);
    gfx_pixelbuffer_set_active(&pixelbuffer);
    gfx_draw_text(00,10,"Hello World!",COL_BLACK,COL_WHITE);

    mh_init();
    mh_gfx_init();

    // put the pixelbuffer in the render-queue to be rendered at all
    // gfx_renderqueue_add_id(pixelbuffer.obj_id);
    gfx_renderqueue_add_id(mh_rs.spritebuffer_id);
    gfx_renderqueue_apply();


}


// █░█ █▀█ █▀▄ ▄▀█ ▀█▀ █▀▀P
// █▄█ █▀▀ █▄▀ █▀█ ░█░ ██▄

bool mouse_actionkey_pressed = false;

void mod_update() {
    io_before_tick();

    mouse_actionkey_pressed = mouse_actionkey_pressed | bit_is_set_some(*mouse_btn_state_pressed,MOUSE_BTN_LEFT);
    gfx_sprite_t* crosshair = mh_rs.crosshair;
    if (crosshair->x != *mouse_x || crosshair->y != *mouse_y){
        crosshair->x = *mouse_x;
        crosshair->y = *mouse_y;
        gfx_sprite_apply_data(crosshair);
    }
    //printf("btn:%d\n",*mouse_btn_state_pressed);

    if (*ms_delta<TICK_RATE)
    {
        return;
    }    
    // the tick goes here!
    if (io_keyboard_is_pressed(HID_KEY_SPACE)){
        current_color_index++;
        gfx_draw_text(10,10,"Hello World95!",current_color_index,COL_WHITE);
    }

    //printf("%d:%d\n",*mouse_x,*mouse_y);




    if (mouse_actionkey_pressed) {
        mouse_actionkey_pressed = false;
        
        uint8_t animator_id = gfx_sprite_get_animator(mh_rs.crosshair);
        bool found_animator = animator_id != 255;
        
        ASSERT_STRICT(found_animator);
        if (found_animator){
            gfx_spriteanimator_restart(animator_id);
            mh_shoot_at(*mouse_x,*mouse_y);
        }

    }

    mh_tick();
    mh_update_huhn_positions();

    int16_t dt = *ms_delta;
    gfx_spritebuffer_update(dt,mh_rs.spritebuffer_id);
    *ms_delta = 0;
}

void mod_render() {

}

#ifndef _MOD_NATIVE_
int main(){
    mod_init();

    while(1){
        mod_update();
    }
}
#endif