#include "game.h"

#include <stdint.h>
#include <stdbool.h>

#include "../ng_gfx.h"
#include "../ng_io.h"
#include "../api/ng_api.h"
#include "../ng_utils.h"

// assets
#include "../../mod/export/font.h"
#include "../../mod/export/color_palette.h"
#include "../../mod/export/color_palette_small.h"
#include "../../mod/export/sprites_misc.h"
#include "../../mod/export/old_guy.h"

int16_t posx=20;
int16_t posy=20;
uint8_t current_col=COL_BLACK;

bool pal_small = false;

gfx_tilesheet_t ts_sprites_misc;
gfx_tilesheet_t ts_old_guy;

const void* assets[]={
    bin2c_font8_bin,
    color_palette,
    sprites_misc,
    color_palette_small,
    gold_guy
};


uint8_t col;
int8_t dir=1;
uint16_t counter = 0;
gfx_sprite_t* sprite;
gfx_sprite_t* sprite_oldguy;

void game_init()
{
    gfx_set_palette_from_assset(ASSETID_PALETTE,0);
    gfx_set_font_from_asset(ASSETID_FONT);

    
    gfx_tilesheet_t* ts = asset_get_pointer(ASSETID_SPRITE_MISC);
    ts_sprites_misc = *ts;
    ts_sprites_misc.tilesheet_data_flash = (uint8_t*)&ts->tilesheet_data_flash + sizeof(uint8_t*);

    ts = asset_get_pointer(ASSETID_SPRITE_OLD_GUY);
    ts_old_guy = *ts;
    ts_old_guy.tilesheet_data_flash = (uint8_t*)&ts->tilesheet_data_flash + sizeof(uint8_t*);

   
    sprite = gfx_sprite_create_from_tilesheet(100,100,&ts_sprites_misc);
    sprite_oldguy = gfx_sprite_create_from_tilesheet(150,150,&ts_old_guy);
}


uint8_t old_guy_id=0;

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
        gfx_tile_set_color(10,10,col);
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
        gfx_set_palette_from_assset(pal_small ? ASSETID_PALETTE_SMALL : ASSETID_PALETTE,0);
    }
    if (io_keyboard_is_pressed(HID_KEY_ENTER)){
        if (sprite->tile_id<7){
            gfx_sprite_set_tileid(sprite,sprite->tile_id+1);
        } else {
            gfx_sprite_set_tileid(sprite,0);
        }
    }
    bool paint = false;
    if (io_keyboard_is_pressed(HID_KEY_A)){
        if (posx>0){
            posx--;
        }
        paint = true;
    }
    else if (io_keyboard_is_pressed(HID_KEY_D)){
        if (posx<40){
            posx++;
        }
        paint = true;
    }
    else if (io_keyboard_is_pressed(HID_KEY_W)){
        if (posy>0){
            posy--;
        }
        paint = true;
    }
    else if (io_keyboard_is_released(HID_KEY_S)){
        if (posy<30){
            posy++;
        }
        paint = true;
    }
    if (paint){
        //gfx_tile_set_color(posx,posy,current_col);
        sprite->x = posx*8;
        sprite->y = posy*8;
        current_col++;
    }    
}