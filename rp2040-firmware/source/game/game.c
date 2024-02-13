#include "game.h"

#include <stdint.h>
#include <stdbool.h>
#include "../core/memory.h"

#include "../ng_gfx.h"
#include "../ng_io.h"
#include "../api/ng_api.h"
#include "../ng_utils.h"
#include <assert.h>

// assets
#include "../../mod/export/font.h"
#include "../../mod/export/color_palette.h"
#include "../../mod/export/color_palette_small.h"
#include "../../mod/export/sprites_misc.h"
#include "../../mod/export/old_guy.h"

int16_t posx=0;
int16_t posy=0;
uint8_t current_col=COL_BLACK;

bool pal_small = false;

gfx_tilesheet_t ts_sprites_misc;
gfx_tilesheet_t ts_old_guy;

const uint8_t* assets[]={
    bin2c_font8_bin,
    color_palette,
    bin2c_sprites_misc_bin,
    color_palette_small,
    bin2c_old_guy_bin
};


uint8_t col;
int8_t dir=1;
uint16_t counter = 0;
gfx_sprite_t* sprite;
gfx_sprite_t* sprite_oldguy;
uint8_t old_guy_id=0;
gfx_pixelbuffer_t* pb;

extern gfx_pixelbuffer_t initial_pixelbuffer;

gfx_pixelbuffer_t second_pixel_buffer={
    .width=100,
    .height=100,
    .x=10,
    .y=200
};

gfx_sprite_buffer_t sprite_buffer = {
    .max_sprites = 16
};

void game_init()
{
    gfx_set_palette_from_assset(ASSETID_PALETTE,0);
    gfx_set_font_from_asset(ASSETID_FONT);

    bool success = gfx_pixelbuffer_create(SEGMENT_GFX_DATA, &second_pixel_buffer);
    assert(success);

    success = gfx_spritebuffer_create(SEGMENT_GFX_DATA, &sprite_buffer);

    gfx_renderqueue_add((ng_mem_block_t*)&initial_pixelbuffer);
    gfx_renderqueue_add((ng_mem_block_t*)&sprite_buffer);
    gfx_renderqueue_add((ng_mem_block_t*)&second_pixel_buffer);
    gfx_renderqueue_apply();

    gfx_pixelbuffer_set_active(&second_pixel_buffer);
    gfx_tile_set_color(0,0,COL_ORANGE);
    gfx_tile_set_color(12,0,COL_ORANGE);
    gfx_tile_set_color(12,12,COL_ORANGE);
    gfx_tile_set_color(0,12,COL_ORANGE);

    gfx_pixelbuffer_set_active(&initial_pixelbuffer);

    gfx_tilesheet_t* ts_sprites_misc = asset_get_tilesheet(ASSETID_SPRITE_MISC);
    sprite = gfx_sprite_create_from_tilesheet(&sprite_buffer, 100,100,ts_sprites_misc);
    // ts_sprites_misc = *ts;
    // ts_sprites_misc.tilesheet_data_flash = (uint8_t*)&ts->tilesheet_data_flash + sizeof(uint8_t*);

    gfx_tilesheet_t* ts_old_guy = asset_get_tilesheet(ASSETID_SPRITE_OLD_GUY);
    sprite_oldguy = gfx_sprite_create_from_tilesheet(&sprite_buffer,150,150,ts_old_guy);
    // ts_old_guy = *ts;
    // ts_old_guy.tilesheet_data_flash = (uint8_t*)&ts->tilesheet_data_flash + sizeof(uint8_t*);
}


void game_tick(int dt)
{
    if (counter > 100){
        counter -= 100;
        col += dir;
        if (dir < 0 && col < 32){
            dir = 1;
            col = 32;
        } else if (dir > 0 && col > 39) {
            dir = -1;
            col = 39;
        }
   //     gfx_tile_set_color(10,10,col);
        old_guy_id++;
        if (old_guy_id>=12){
            old_guy_id=0;
        }
        gfx_sprite_set_tileid(sprite_oldguy,old_guy_id);
    } else {
        counter += dt;
    }

    if (io_keyboard_is_pressed(HID_KEY_0)){
        pal_small = !pal_small;
        gfx_pixelbuffer_set_active(pal_small ? &second_pixel_buffer : &initial_pixelbuffer);

        //gfx_set_palette_from_assset(pal_small ? ASSETID_PALETTE_SMALL : ASSETID_PALETTE,0);
    }
    if (io_keyboard_is_pressed(HID_KEY_ENTER)){
        if (sprite->tile_id<7){
            gfx_sprite_set_tileid(sprite,sprite->tile_id+1);
        } else {
            gfx_sprite_set_tileid(sprite,0);
        }
    }
    //#define NO_BOUNDCHECK

    bool paint = false;
    if (io_keyboard_is_down(HID_KEY_A)){
#ifdef BOUNDCHECK
        if (posx>0)
#endif        
        {
            posx--;
        }
        paint = true;
    }
    else if (io_keyboard_is_down(HID_KEY_D)){
#ifdef BOUNDCHECK
        if (posx<40)
#endif        
        {
            posx++;
        }
        paint = true;
    }
    else if (io_keyboard_is_down(HID_KEY_W)){
#ifdef BOUNDCHECK
        if (posy>0)
#endif        
        {
            posy--;
        }
        paint = true;
    }
    else if (io_keyboard_is_down(HID_KEY_S)){
#ifdef BOUNDCHECK
        if (posy<30)
#endif        
        {
            posy++;
        }
        paint = true;
    }
    if (paint){
        //gfx_tile_set_color(posx,posy,current_col);
        // sprite->x = posx*8;
        // sprite->y = posy*8;
        pb = gfx_pixelbuffer_get_current();
        pb->x = posx;
        pb->y = posy;
        current_col++;

    }    
}