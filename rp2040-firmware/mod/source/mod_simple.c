
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
volatile uint8_t* pxbuf = NULL;
volatile gamepad_t* gamepad = NULL;
volatile gamepad_state_t* gamepad_state = NULL;

// #define SCREEN_WIDTH 80
// #define SCREEN_HEIGHT 60
#define SCREEN_WIDTH 160
#define SCREEN_HEIGHT 120
// #define PX 1
// #define PY 1
#define PX (320/SCREEN_WIDTH)
#define PY (240/SCREEN_HEIGHT)
#define TICK_RATE (1000/30)
volatile uint16_t* ms_delta = NULL;

uint8_t px_width;
uint8_t px_height;


int16_t x=0;

#define KEY_COL_DOWN    (1 << 7)
#define KEY_COL_UP      (1 << 6)
#define KEY_LEFT        (1 << 5)
#define KEY_LEFT2       (1 << 4)
#define KEY_RIGHT2      (1 << 3)
#define KEY_RIGHT       (1 << 2)


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
    .flags = KEYBMAP_FLAG_SCAN_KEY_PRESSED | KEYBMAP_FLAG_SCAN_KEY_RELEASED
};

// tilemap
gfx_tilesheet_data_t ts_spritesheet;
gfx_tilemap_t tilemap = {0};

typedef struct gfx_layer_30_30_t {
    gfx_tilemap_layer_t layer_data;
    uint8_t data[30*30];
} gfx_layer_30_30_t;

gfx_layer_30_30_t layer0 = {0};


// pixelbuffer
gfx_pixelbuffer_t pixelbuffer = {
    .width=SCREEN_WIDTH,
    .height=SCREEN_HEIGHT,
    .x=0,
    .y=0,
    .pixel_size=flags_pack_4_4(PX,PY),
    //.flags=PXB_WRAPMODE(0,PXB_WRAPMODE_WRAP)
};

// spritebuffer
uint8_t spritebuffer;
#define SPRITE_AMOUNT 4
gfx_sprite_t sprites[SPRITE_AMOUNT];
gfx_tilesheet_data_t ts_misc;
uint8_t powerup_sound = 0;

int mod_init(){
    powerup_sound = audio_wav_load(ASSET_POWER_UP_8);

    io_keyboardmapping_register(&kbm,1);

    //audio_play_wav(ASSET_MUSIC_8,true);
   // audio_mod_play(ASSET_GAME);

    ms_delta = (uint16_t*)MEMPTR(MM_MS_DELTA);
    mx =  (uint16_t*)MEMPTR(MM_MOUSE_X);
    my =  (uint16_t*)MEMPTR(MM_MOUSE_Y);
    mbtn = (uint8_t*)MEMPTR(MM_MOUSE_BTN);
    gamepad = (gamepad_t*)MEMPTR(MM_GAMEPAD);
    gamepad_state = (gamepad_state_t*)MEMPTR(MM_GAMEPAD1_STATE);

    gfx_set_font_from_asset(ASSET_FONT8);
    gfx_set_palette_from_assset(ASSET_COLOR_PALETTE,0);

    gfx_pixelbuffer_create(&pixelbuffer);
    gfx_pixelbuffer_set_active(&pixelbuffer);

    gfx_draw_pixel(0,0,COL_RED);
    gfx_draw_pixel(SCREEN_WIDTH-1,0,COL_RED);
    gfx_draw_pixel(SCREEN_WIDTH-1,SCREEN_HEIGHT-1,COL_RED);
    gfx_draw_pixel(0,SCREEN_HEIGHT-1,COL_RED);

    // tilemap
    asset_get_tilesheet(&ts_spritesheet,ASSET_SPRITESHEET);
    asset_get_tilemap(&tilemap,ASSET_TILEMAP);
    tilemap.tilesheet_id=ts_spritesheet.ts_id;
    gfx_load_tilemap_layer(&tilemap,&layer0,0);
    gfx_draw_tilemap_layer(10,10,&layer0);

    // sprite
    asset_get_tilesheet(&ts_misc,ASSET_SPRITES_MISC);
    spritebuffer = gfx_spritebuffer_create(sprites,SPRITE_AMOUNT);

    for (uint8_t i=0; i<SPRITE_AMOUNT;i++){ 
        gfx_sprite_set_tileset(&sprites[i], &ts_misc,i%4);
        sprites[i].x=(i % 4) * 60;
        sprites[i].y=(i / 4) * 30;
        sprites[i].pixel_size=flags_pack_4_4(2,2);
    }


    // renderqueue
    gfx_renderqueue_add_id(pixelbuffer.obj_id);
 //   gfx_renderqueue_add_id(spritebuffer);

    gfx_renderqueue_apply();

    flags_unpack_4_4(pixelbuffer.pixel_size,px_width,px_height);
    int a=0;
}

uint16_t last_x =0;
int8_t dir = 1;


bool mod_playing = true;

void mod_update() {
    // TODO: implement some kind of sleep
    if (*ms_delta<TICK_RATE)
    {
        return;
    }
    char buf[50];
/*
  int8_t  x;         ///< Delta x  movement of left analog-stick
  int8_t  y;         ///< Delta y  movement of left analog-stick
  int8_t  z;         ///< Delta z  movement of right analog-joystick
  int8_t  rz;        ///< Delta Rz movement of right analog-joystick
  int8_t  rx;        ///< Delta Rx movement of analog left trigger
  int8_t  ry;        ///< Delta Ry movement of analog right trigger
  uint8_t hat;       ///< Buttons mask for currently pressed buttons in the DPad/hat
  uint16_t buttons;  ///< Buttons mask for currently pressed buttons
  uint16_t buttons2;
  */

    ng_snprintf(buf,40,"x%d y%d z%d",gamepad->x,gamepad->y,gamepad->z);
    gfx_draw_text(0,0,buf,COL_ORANGE);

    ng_snprintf(buf,40,"rz%d rx%d ry%d",gamepad->rz,gamepad->rx,gamepad->ry);
    gfx_draw_text(0,10,buf,COL_ORANGE);

    ng_snprintf(buf,40,"h%d b%d b%d",gamepad->hat,gamepad->buttons,gamepad->buttons2);
    gfx_draw_text(0,20,buf,COL_ORANGE);

    ng_snprintf(buf,40,"c:%d b:%d",gamepad_state->controls,gamepad_state->buttons);
    gfx_draw_text(0,40,buf,COL_ORANGE);

    if ((kbm.key_pressed & KEY_COL_DOWN)>0){
        x--;
        // if (px_width>0){
        //     px_width--;
        // }
        //pixelbuffer.x--;
        
        audio_wav_play(powerup_sound,false);
    }

    if ((kbm.key_pressed & KEY_COL_UP)>0){
        if (mod_playing){
            audio_mod_pause();
        } else {
            audio_mod_resume();
        }
        mod_playing = !mod_playing;
    }


    x += dir;

    if (dir==1 && x > 300){
        dir = -1;

    }
    if (dir == -1 && x < 20){
        dir = 1;
    }

    pixelbuffer.x++;
    sprites[0].x=x;
    gfx_sprite_apply_data(&sprites[0]);
    //gfx_pixelbuffer_apply_data(&pixelbuffer);
    
    int16_t dt = *ms_delta;
    *ms_delta=0;

    bool changed = false;

   
    //flags_set(pixelbuffer.flags,PXBFLAG_DIRTY);
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