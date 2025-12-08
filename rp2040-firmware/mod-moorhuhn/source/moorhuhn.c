#include "moorhuhn.h"
#include "mh_globals.h"
#include "export/assets.h"

#ifdef _MOD_NATIVE_
# include "../../source/core/memory.h"
# include "../../source/ng_all.h"
#else
# include <ng_config.h>
# include "../../source/ng_all.h"
#endif

uint8_t audio_powerup, audio_crash;
mh_gamestate_t mh_gs;


void change_hits(int16_t dt);
void change_ammo(int16_t dt);


void init_audio(void){
    audio_mod_play(ASSET_GAME);

    audio_crash = audio_wav_load(ASSET_CRASH);
    audio_powerup = audio_wav_load(ASSET_POWER_UP_8);
}

void mh_huhn_random_values(mh_huhn_t* moorhuhn){
    *moorhuhn = (mh_huhn_t){
        .x = utils_random_uint16() % (320*2),
        .y = utils_random_uint16() % 200,
        .velocity = (utils_random_uint16() % 7) - 3,
        .flags = MH_HUHNFLAG_ALIVE,
        .sin_multi = flags_pack_4_4((utils_random_uint16() % 3)+1, (utils_random_uint16() % 2)+1),
        .hitpoints = 1,
    };
    utils_random_uint16();
    if (moorhuhn->velocity == 0){
        moorhuhn->velocity = (utils_random_uint16() % 2) == 0 ? 1 : -1;        
    }
}

void mh_start_game() {
    mh_gs = (mh_gamestate_t){0};
    change_ammo(10);
    init_audio();
}

void mh_init() {
    mh_start_game();
}

void mh_tick_mhs(){

}

void mh_tick() {
    mh_huhn_t* mh = &mh_gs.mhs[0];
    for (uint8_t i=0,iEnd=mh_gs.mhs_amount; i<iEnd; i++){
        ASSERT_STRICT(flags_isset(mh->flags,MH_HUHNFLAG_ALIVE));

        mh->x += mh->velocity;
        if (mh->x > (320+160)){
            mh->x = 0;
        } else if (mh->x<0){
            mh->x = (320+160);
        }
        mh++;
    }
}

void mh_update_huhn_positions();



bool mh_huhn_find_free(uint8_t* huhn_idx, mh_huhn_t** huhn){
    bool huhns_available = mh_gs.mhs_amount < MAX_MOORHUHN_AMOUNT;
    if (!huhns_available) {
        return false;
    }
    *huhn = &mh_gs.mhs[mh_gs.mhs_amount];
    *huhn_idx = mh_gs.mhs_amount;
    mh_gs.mhs_amount++;
    return true;
}

void mh_huhn_remove_at_idx(uint8_t idx){
    ASSERT_STRICT(idx < mh_gs.mhs_amount);
    uint8_t current_amount = --mh_gs.mhs_amount;
    bool is_last_element = idx == current_amount;
    if (!is_last_element){
        mh_gs.mhs[idx]=mh_gs.mhs[current_amount];
    }
}

bool mh_huhn_remove(mh_huhn_t* huhn){
    // TODO: I can actually calculate the position,right?
    for (int i=0,iEnd=mh_gs.mhs_amount;i<iEnd;i++){
        if ((&mh_gs.mhs[i])==huhn){
            mh_huhn_remove_at_idx(i);
            return true;
        }
    }
    return false;
}


bool mh_huhn_spawn(mh_huhn_t* huhn_data, uint8_t type, mh_huhn_t** out_huhn){
    // find free-huhn-data
    mh_huhn_t* huhn;
    uint8_t huhn_idx;
    bool found_free_huhn = mh_huhn_find_free(&huhn_idx,&huhn);
    if (!found_free_huhn){
        return false;
    }

    // find
    uint8_t sprite_id = gfx_spritebuffer_find_free_sprite(mh_rs.spritebuffer_id);
    bool found_free_sprite = sprite_id != 255;
    if (!found_free_sprite){
        mh_huhn_remove(huhn);
        return false;
    }
    gfx_sprite_t* sprite = &mh_rs.sprites[sprite_id];
    
    *huhn = *huhn_data;
    huhn->sprite_id = sprite_id;
    switch (type){
        default:
            gfx_sprite_set_tileset(sprite,&mh_rs.ts_bird,0);
            sprite->pixel_size=flags_pack_4_4(1,1);
            uint8_t sprite_anim = gfx_sprite_add_animator(sprite, &anim4x1);
            gfx_spriteanimator_set_animation(sprite_anim, 0, ANIMATIONFLAG_LOOP);
            break;
    }
        
    sprite->flags |= SPRITEFLAG_ALIGNH_LEFT | SPRITEFLAG_ALIGNV_TOP | SPRITEFLAG_ENABLED | SPRITEFLAG_IN_USE;     

    if (huhn->velocity < 0){
        sprite->flags |= SPRITEFLAG_FLIP_H;
    }        
    gfx_sprite_apply_data(sprite);

    *out_huhn = huhn;
    return true;
}

void mh_update_huhn_positions() {
    uint8_t sin_x_scale;
    uint8_t sin_y_scale;
    for (uint8_t i=0,iEnd=mh_gs.mhs_amount;i<iEnd;i++){
        mh_huhn_t* huhn = &mh_gs.mhs[i];
        gfx_sprite_t* sprite = &mh_rs.sprites[huhn->sprite_id];
        sprite->x = mh_rs.view_x+huhn->x;

        flags_unpack_4_4(huhn->sin_multi,sin_x_scale,sin_y_scale);
        int16_t y_offset = sine_table[val_to_sin_idx(mh_rs.tick_counter * sin_x_scale)*1];
        sprite->y = huhn->y + y_offset;
        gfx_sprite_apply_data(sprite);
    }
}

void change_hits(int16_t dt){
    mh_gs.hits += dt;
    flags_set(mh_gs.flags, GS_FLAG_UIDATA_DIRTY);
}

void change_ammo(int16_t dt){
    mh_gs.ammo += dt;
    flags_set(mh_gs.flags, GS_FLAG_UIDATA_DIRTY);
}

bool mh_shoot_at(int16_t x,int16_t y) {
    if (mh_gs.ammo == 0){
        return false;
    }
    // TODO no ammo on powerup
    change_ammo(-1);

    for (uint8_t i=0,iEnd=mh_gs.mhs_amount;i<iEnd;i++){
        mh_huhn_t* huhn = &mh_gs.mhs[i];
        gfx_sprite_t* sprite = &mh_rs.sprites[huhn->sprite_id];
        
        bool hit = gfx_sprite_intersect_with_point(sprite, x,y);

        if (hit) {
            change_ammo(+2);
            change_hits(+1);
            audio_wav_play(audio_powerup,false);
            
            uint8_t animator_id = gfx_sprite_get_animator(sprite);
            gfx_spriteanimator_set_animation(animator_id,1,ANIMATIONFLAG_FREE_SPRITE_ON_STOP);
            mh_huhn_remove_at_idx(i);
            return true;
        } 

        // uint8_t psx,psy;
        // flags_unpack_4_4(sprite->pixel_size, psx,psy);

        // gfx_internal_spritebuffer_t* spritebuffer = id_get_ptr(sprite->spritebuffer_id);
        // gfx_internal_sprite_t *si = spritebuffer->sprite_internals;

        // if (    x < sprite->x 
        //     ||  y < sprite->y 
        //     ||  x > sprite->x + si->sprite_width 
        //     ||  y > sprite->y + si->sprite_height
        // ){
        //     continue;
        // }

        // uint8_t animator_id = gfx_sprite_get_animator(sprite);
        // gfx_spriteanimator_set_animation(animator_id,1,0);

        // flags_unset(sprite->flags,SPRITEFLAG_ENABLED|SPRITEFLAG_IN_USE);
        // huhn->sprite_id=255;
        // mh_huhn_remove_at_idx(i);            
        // gfx_sprite_apply_data(sprite);
    }
    audio_wav_play(audio_crash,false);
    return false;
}
