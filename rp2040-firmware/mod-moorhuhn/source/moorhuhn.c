#include "moorhuhn.h"
#include "mh_globals.h"

#ifdef _MOD_NATIVE_
# include "../../source/core/memory.h"
# include "../../source/ng_all.h"
#else
# include <ng_config.h>
# include "../../source/ng_all.h"
#endif



mh_gamestate_t mh_gs;

void mh_huhn_new(mh_huhn_t* moorhuhn){
    *moorhuhn = (mh_huhn_t){
        .x = utils_random_uint16() % 320,
        .y = utils_random_uint16() % 200,
        .velocity = (utils_random_uint16() % 2)==0 ? -1 : +1,
        .flags = MH_HUHNFLAG_ALIVE,
        .hitpoints = 1,
    };
}

void mh_start_game() {
    mh_gs = (mh_gamestate_t){0};
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
        if (mh->x > 320){
            mh->x = 0;
        } else if (mh->x<0){
            mh->x = 320;
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
    for (uint8_t i=0,iEnd=mh_gs.mhs_amount;i<iEnd;i++){
        mh_huhn_t* huhn = &mh_gs.mhs[i];
        gfx_sprite_t* sprite = &mh_rs.sprites[huhn->sprite_id];
        sprite->x = huhn->x;
        sprite->y = huhn->y;
        gfx_sprite_apply_data(sprite);
    }
}
