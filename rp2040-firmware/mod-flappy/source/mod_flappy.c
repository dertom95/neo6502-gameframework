
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

#include "flappy.h"

// peeks'n'pokes
// mouse btn
volatile uint8_t* mbtn = NULL;
volatile uint16_t* ms_delta = NULL;
volatile gamepad_state_t* gamepad_pressed = NULL;
volatile uint8_t* mouse_btn_state_pressed = NULL;


#define TICK_RATE (1000/30)
#define delay 120


#define KEY_ACTION    (1 << 7)
#define KEY_ACTION_2    (1 << 6)

keyboard_mapping_t kbm={
    .keycodes = {
        HID_KEY_SPACE,
        HID_KEY_0,
        0,
        0,
        0,
        0,
        0,
        0},
    .flags = KEYBMAP_FLAG_SCAN_KEY_PRESSED | KEYBMAP_FLAG_SCAN_KEY_DOWN | KEYBMAP_FLAG_SCAN_KEY_RELEASED
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

gfx_pixelbuffer_t pixelbuffer_bg = {
    .width=64,
    .height=80,
    .x=0,
    .y=0,
    .pixel_size=flags_pack_4_4(5,3),
    //.flags=PXB_WRAPMODE(0,PXB_WRAPMODE_WRAP)
};

gfx_pixelbuffer_t pixelbuffer_ui = {
    .width=240,
    .height=20,
    .x=0,
    .y=0,
    .pixel_size=flags_pack_4_4(1,1),
    //.flags=PXB_WRAPMODE(0,PXB_WRAPMODE_WRAP)
};

// 1 bird and two columns with 2 pillars each
#define PILLAR_AMOUNT 4
#define SPRITE_AMOUNT (1 + PILLAR_AMOUNT)
gfx_sprite_t sprites[SPRITE_AMOUNT];

gfx_sprite_t* sprite_bird=&sprites[4];
gfx_sprite_t* sprite_pillars[PILLAR_AMOUNT]={
    &sprites[0],
    &sprites[1],
    &sprites[2],
    &sprites[3]
};

gfx_tilesheet_data_t ts_bird,ts_pillar,ts_bg;

int8_t anim=0;


uint8_t last_gamestate=0;
uint8_t last_score = 0;


int16_t x=0;

void init_system();
void init_gfx();
void init_audio();


// █ █▄░█ █ ▀█▀
// █ █░▀█ █ ░█░

int mod_init(){
    init_system();
    init_gfx();
    init_audio();

    flappy_init();
}

void init_gfx_bird(){
    gfx_sprite_set_tileset(sprite_bird,&ts_bird,0);
    sprite_bird_anim = gfx_sprite_add_animator(sprite_bird,&anim4x1);
    gfx_spriteanimator_set_animation(sprite_bird_anim, 0, /*ANIMATIONFLAG_STOPPED |*/ ANIMATIONFLAG_LOOP);
    sprite_bird->x=120;
    sprite_bird->y=120;
    sprite_bird->pixel_size=flags_pack_4_4(2,2);
    sprite_bird->flags = SPRITEFLAG_ALIGNH_LEFT | SPRITEFLAG_ALIGNV_TOP;
    gfx_sprite_apply_data(sprite_bird);
}

void init_gfx_pillars(){
    for (uint8_t i=0; i < PILLAR_AMOUNT; i++){
        gfx_sprite_t* pillar = sprite_pillars[i];
        gfx_sprite_set_tileset(pillar,&ts_pillar,0);
        pillar->x=i * 40;
        pillar->y=10;
        pillar->pixel_size=flags_pack_4_4(1,3);
        bit_set(pillar->flags, SPRITEFLAG_ALIGNV_TOP | SPRITEFLAG_ALIGNH_LEFT);
        gfx_sprite_apply_data(pillar);
    }
}

void init_gfx_background() {
    gfx_tilesheet_current_set(&ts_bg);
    gfx_draw_tile(0,0,0);
}

void init_gfx_ui() {
    gfx_tilesheet_current_set(&ts_bg);
    gfx_draw_tile(0,0,0);
}

void init_gfx() {
    // font
    gfx_set_font_from_asset(ASSET_FONT8);
    gfx_set_palette_from_assset(ASSET_COLOR_PALETTE,0);

    // sprint & bg
    spritebuffer = gfx_spritebuffer_create(sprites,SPRITE_AMOUNT);

    gfx_pixelbuffer_create(&pixelbuffer_bg);
    gfx_pixelbuffer_create(&pixelbuffer_ui);
    gfx_pixelbuffer_set_active(&pixelbuffer_bg);


    asset_get_tilesheet(&ts_bird,ASSET_SPRITE_BIRD);
    asset_get_tilesheet(&ts_pillar,ASSET_SPRITE_PILLAR);
    asset_get_tilesheet(&ts_bg,ASSET_BITMAP_BACKGROUND);
    
    gfx_tilesheet_current_set(&ts_bg);

    init_gfx_bird();
    init_gfx_pillars();
    init_gfx_background();
    init_gfx_ui();

    gfx_pixelbuffer_set_active(&pixelbuffer_ui);

    // render objects and order! 
    gfx_renderqueue_add_id(pixelbuffer_bg.obj_id);
    gfx_renderqueue_add_id(spritebuffer);
    gfx_renderqueue_add_id(pixelbuffer_ui.obj_id);
    gfx_renderqueue_apply();
}

void init_system(){
    // TODO: assignment at declaration?
    ms_delta = (uint16_t*)MEMPTR(MM_MS_DELTA);
    mbtn = (uint8_t*)MEMPTR(MM_MOUSE_BTN);
    gamepad_pressed = (gamepad_state_t*)MEMPTR(MM_GAMEPAD1_STATE_PRESSED);
    mouse_btn_state_pressed = MEMPTR(MM_MOUSE_BTN_PRESSED);
    io_keyboardmapping_register(&kbm,1);
}

void init_audio(void){
    audio_mod_play(ASSET_GAME);
}

uint8_t hit_counter = 0;

// █░█ █▀█ █▀▄ ▄▀█ ▀█▀ █▀▀
// █▄█ █▀▀ █▄▀ █▀█ ░█░ ██▄

void mod_update() {
    // TODO: implement some kind of sleep


    
    //bool mouse_actionkey_pressed = bit_is_set_all(mbtn,);
    char buf[41];
    ng_snprintf(buf,41,"down:%s keyb:%d gp:%d hc:%d ",io_keyboard_is_released(HID_KEY_0)?"yes":"no",kbm.key_down, gamepad_pressed->buttons,hit_counter);
    gfx_draw_text(0,10,buf,COL_BLACK,COL_WHITE);
    
    bool keyboard_actionkey_pressed = io_keyboard_is_released(HID_KEY_0);
    bool gamepad_actionkey_pressed = bit_is_set_some(gamepad_pressed->buttons,0xff);
    bool mouse_actionkey_pressed = bit_is_set_some(*mouse_btn_state_pressed,MOUSE_BTN_LEFT);

    if (keyboard_actionkey_pressed || gamepad_actionkey_pressed || mouse_actionkey_pressed){
        hit_counter++;
        flappy_on_actionbutton();
    }    

    if (*ms_delta<TICK_RATE)
    {
        return;
    }


    io_input_clear_states();    

    int16_t dt = *ms_delta;
    gfx_spritebuffer_update(dt,spritebuffer);
    *ms_delta=0;

    static uint8_t seed = 0;
    seed++;

    bool changed = false;

    flappy_tick();
}



void draw_stuff()
{
    gamedata_t* gd = get_gamedata();

    if (gd->player_vel > 5.0f){
        gfx_spriteanimator_stop(sprite_bird_anim);
    } else {
        gfx_spriteanimator_resume(sprite_bird_anim);
    }

    //draw pipes
    for(int i = 0; i < 2; i++)
    {
        gfx_sprite_t* pillar_sprite_top = sprite_pillars[2*i];
        gfx_sprite_t* pillar_sprite_bottom = sprite_pillars[2*i + 1];
        
        pillar_sprite_top->x = gd->pipe_x[i];
        pillar_sprite_top->y = gd->pipe_y[i] - H;
        gfx_sprite_apply_data(pillar_sprite_top); // TODO: gdx:why apply? 

        pillar_sprite_bottom->x = gd->pipe_x[i];
        pillar_sprite_bottom->y = gd->pipe_y[i] + GAP;
        gfx_sprite_apply_data(pillar_sprite_bottom);
    }

    sprite_bird->x = PLYR_X;
    sprite_bird->y = gd->player_y;
    gfx_sprite_apply_data(sprite_bird);

    char buf[41];

    if(gd->gamestate == GS_ALIVE && (last_gamestate!=GS_ALIVE || last_score!=gd->score)) {
        gfx_pixelbuffer_wipe(&pixelbuffer_ui,COL_TRANSPARENT);
        if (last_gamestate != GS_ALIVE){
            pixelbuffer_ui.x = 0;
            pixelbuffer_ui.y = 0;
            gfx_pixelbuffer_apply_data(&pixelbuffer_ui);
        }
        ng_snprintf(buf,sizeof(buf),"score:%d  high:%d",gd->score,gd->best);
        gfx_draw_text(0,0,buf,COL_ORANGE_DARK,COL_TRANSPARENT);
        last_score = gd->score;
    }
    else if(gd->gamestate == GS_READY && last_gamestate!=GS_READY) {
        gfx_pixelbuffer_wipe(&pixelbuffer_ui,COL_TRANSPARENT);
        gfx_draw_text(0,0,"Press any key",COL_ORANGE,COL_GREY);
    }
    else if(gd->gamestate == GAMEOVER && last_gamestate!=GAMEOVER) {
        gfx_pixelbuffer_wipe(&pixelbuffer_ui,COL_TRANSPARENT);
        ng_snprintf(buf,40,"Nice Try! Score: %d Higscore: %d",gd->score,gd->best);
        gfx_draw_text(0,0,buf,COL_ORANGE_DARK,COL_GREY);
        pixelbuffer_ui.x = 40;
        pixelbuffer_ui.y = 115;
        gfx_pixelbuffer_apply_data(&pixelbuffer_ui);
    }
    else if(gd->gamestate == GAMEOVER_HIGHSCORE && last_gamestate!=GAMEOVER_HIGHSCORE) {
        gfx_pixelbuffer_wipe(&pixelbuffer_ui,COL_TRANSPARENT);
        ng_snprintf(buf,40,"Great! New High score: %d",gd->score);
        gfx_draw_text(0,0,buf,COL_ORANGE_DARK,COL_GREY);
        pixelbuffer_ui.x = 40;
        pixelbuffer_ui.y = 115;
        gfx_pixelbuffer_apply_data(&pixelbuffer_ui);
    }
    
    last_gamestate = gd->gamestate;
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