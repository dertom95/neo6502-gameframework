#include "ng_gfx.h"



// █▀ █▀█ █▀█ █ ▀█▀ █▀▀   █▀█ █▀▀ █░░ ▄▀█ ▀█▀ █▀▀ █▀▄
// ▄█ █▀▀ █▀▄ █ ░█░ ██▄   █▀▄ ██▄ █▄▄ █▀█ ░█░ ██▄ █▄▀

#ifdef PICO_NEO6502
# include "core/backend/neo6502/neo6502.h"
# include "pico/platform.h"
#endif
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "gen/color_palette.h"
//#include "zelda_mini_plus_walk_rgab5515.h"
#include <string.h>

#include "api/gen/ng_api.h"

#include "ng_io.h"
#include <math.h>

#include "ng_utils.h"
#include <assert.h>
#include "core/memory.h"
#include "ng_assets.h"
#include "ng_utils.h"

void _internal_gfx_ext_update_spriteanimator(gfx_internal_sprite_animator_t* animator, gfx_sprite_t* sprite, uint16_t dt);
void _internal_gfx_sprite_update(gfx_sprite_t* sprite, gfx_internal_sprite_t* sprite_internal, uint16_t dt);

// 🇸​​​​​🇵​​​​​🇷​​​​​🇮​​​​​🇹​​​​​🇪​​​​​🇧​​​​​🇺​​​​​🇫​​​​​🇫​​​​​🇪​​​​​🇷​​​​​

uint8_t gfx_spritebuffer_create(gfx_sprite_t* spritedata,uint8_t spriteamount) {
	assert(spritedata!=NULL && "spritedata must not be null");
    assert(spriteamount>0 && "you cannot create spritebuffer with amount 0");

	assert(ng_mem_segment_space_left(SEGMENT_GFX_DATA) > sizeof(gfx_internal_spritebuffer_t) && "gfx_spritebuffer_create: exceeded space");

    gfx_internal_spritebuffer_t* spritebuffer = ng_mem_allocate(SEGMENT_GFX_DATA, sizeof(gfx_internal_spritebuffer_t));
	spritebuffer->amount_sprites=spriteamount;
    spritebuffer->sprites = spritedata;
    ng_mem_allocate_block(SEGMENT_GFX_DATA,spriteamount * sizeof(gfx_internal_sprite_t), MEM_USAGE_SPRITEBUFFER, &spritebuffer->mem);
    ng_memblock_wipe(&spritebuffer->mem);
    spritebuffer->sprite_internals=(gfx_internal_sprite_t*)spritebuffer->mem.data;

    uint8_t spritebuffer_id = id_store(spritebuffer);

    uint8_t count = spriteamount;
    for (int idx=0;idx<spriteamount;idx++){
        gfx_sprite_t* sprite_current = &spritebuffer->sprites[idx];
        sprite_current->spritebuffer_id = spritebuffer_id;
        sprite_current->sprite_idx=idx;

        //gfx_internal_sprite_t* sprite_internal = &spritebuffer->sprite_internals[idx];
        // nothing to do with internals,yet
    }

    return spritebuffer_id;
}

void  gfx_spritebuffer_update(int16_t dt,uint8_t spritebuffer_id){
    gfx_internal_spritebuffer_t* spritebuffer = id_get_ptr(spritebuffer_id);
    
    uint8_t amount = spritebuffer->amount_sprites;
    gfx_sprite_t* current_sprite = spritebuffer->sprites;
    gfx_internal_sprite_t* current_internal_sprite = spritebuffer->sprite_internals;
    while (amount--){
        if (current_sprite->flags!=0 && current_internal_sprite->extensions!=NULL){
            _internal_gfx_sprite_update(current_sprite,current_internal_sprite,dt);
        }
        current_sprite++;
        current_internal_sprite++;
    }
}

// -------------
// 🇸​​​​​🇵​​​​​🇷​​​​​🇮​​​​​🇹​​​​​🇪​​​​​
// -------------
void _gfx_sprite_set_tileid(gfx_sprite_t* sprite,gfx_internal_sprite_t* sprite_internal, uint8_t tile_idx){
	assert(sprite_internal->tilesheet!=NULL && "no tilesheet set for sprite!");
    assert(tile_idx < sprite_internal->tilesheet->data.tile_amount && "exceeded tile-id");
    
	sprite_internal->tile_ptr = gfx_tilesheet_get_chached_tile(sprite_internal->tilesheet, tile_idx);
	sprite->tile_idx = tile_idx;
}

void gfx_sprite_set_tileset(gfx_sprite_t* sprite, gfx_tilesheet_data_t* tsdata, uint8_t initial_tile_idx) {
    assert(sprite!=NULL);

    gfx_internal_spritebuffer_t* spritebuffer = id_get_ptr(sprite->spritebuffer_id);
    gfx_tilesheet_t* ts = id_get_ptr(tsdata->ts_id);

    gfx_internal_sprite_t* sprite_internal = &spritebuffer->sprite_internals[sprite->sprite_idx];

    sprite_internal->tilesheet = ts;
    sprite->flags = 1;
    _gfx_sprite_set_tileid(sprite, sprite_internal, initial_tile_idx);
}

void gfx_sprite_set_tileid(gfx_sprite_t* sprite,uint8_t tile_idx){
    assert(sprite!=NULL);
    gfx_internal_spritebuffer_t* spritebuffer = id_get_ptr(sprite->spritebuffer_id);

	if (sprite->tile_idx == tile_idx){
		return;
	}
    gfx_internal_sprite_t* sprite_internal = &spritebuffer->sprite_internals[sprite->sprite_idx];

    _gfx_sprite_set_tileid(sprite,sprite_internal,tile_idx);
}

bool gfx_sprite_remove_extension(gfx_sprite_t* sprite,uint8_t extension_type){
    assert(sprite!=NULL);
    gfx_internal_spritebuffer_t* spritebuffer = id_get_ptr(sprite->spritebuffer_id);
    gfx_internal_sprite_t* sprite_internal = &spritebuffer->sprite_internals[sprite->sprite_idx];
    
    gfx_extension_header_t* header = sprite_internal->extensions;
    while (header!=NULL){
        if (header->extension_type==extension_type){
            // TODO: I'm lazy and actually could do it right now! But i'm tired and want to come forth
            ll_remove((linked_list_t**)sprite_internal, (linked_list_t*)header);
            return true;
        }
    }
    return false;
}


void _internal_gfx_sprite_update(gfx_sprite_t* sprite, gfx_internal_sprite_t* sprite_internal, uint16_t dt)
{
    // TODO: add flag that indicates it has extensions to be updated
    gfx_extension_header_t* extension = sprite_internal->extensions;
    while (extension!=NULL){
        switch (extension->extension_type){
            case GFX_EXTENSION_SPRITEANIMATOR:{
                _internal_gfx_ext_update_spriteanimator((gfx_internal_sprite_animator_t*)extension,sprite,dt);
                break;
            }
            default:
                assert(false && "unknown sprite-extension!");
        }
        extension=(gfx_extension_header_t*)extension->next;
    }
}

// █▀▀ ▀▄▀ ▀█▀ █▀▀ █▄░█ █▀ █ █▀█ █▄░█ █▀
// ██▄ █░█ ░█░ ██▄ █░▀█ ▄█ █ █▄█ █░▀█ ▄█


// -----------------
// 🇦​​​​​🇳​​​​​🇮​​​​​🇲​​​​​🇦​​​​​🇹​​​​​🇴​​​​​🇷​​​​​
// -----------------

uint8_t  gfx_sprite_add_animator(gfx_sprite_t* sprite, gfx_sprite_animator_t* animator){
    assert(sprite!=NULL);
    assert(animator!=NULL);
    assert(animator->animation_amount>0);

    gfx_internal_spritebuffer_t* spritebuffer = id_get_ptr(sprite->spritebuffer_id);

    gfx_internal_sprite_animator_t* internal_animator = ng_mem_allocate(SEGMENT_GFX_DATA,sizeof(gfx_internal_sprite_animator_t));
    *internal_animator=(gfx_internal_sprite_animator_t){
        .header={
            .extension_type = GFX_EXTENSION_SPRITEANIMATOR
        },
        .sprite_animator = animator,
        .current_animation = &animator->animations[0],
        .timer=animator->animations[0].delay_ms
    };
    gfx_sprite_set_tileid(sprite, internal_animator->current_animation->start_tile);

    gfx_internal_sprite_t* sprite_internal = &spritebuffer->sprite_internals[sprite->sprite_idx];
    ll_add((linked_list_t**)&sprite_internal->extensions, (linked_list_t*)internal_animator);
    return id_store(internal_animator);
}

void gfx_spriteanimator_set_animation(uint8_t sprite_animator_id, uint8_t anim_idx){
    gfx_internal_sprite_animator_t* internal_spriteanimatior = id_get_ptr(sprite_animator_id);
    if (anim_idx == internal_spriteanimatior->current_anim_idx){
        // nothing todo! TODO: assert this?
        return;
    }

    gfx_sprite_animator_t* usr_animator = internal_spriteanimatior->sprite_animator;
    assert(anim_idx < usr_animator->animation_amount && "idx out of bounds!"); 

    internal_spriteanimatior->current_anim_idx = anim_idx;
    internal_spriteanimatior->current_animation = &usr_animator->animations[anim_idx];
    internal_spriteanimatior->timer = internal_spriteanimatior->current_animation->delay_ms;
}

void _internal_gfx_ext_update_spriteanimator(gfx_internal_sprite_animator_t* animator, gfx_sprite_t* sprite, uint16_t dt){
    animator->timer -= dt;
    if (animator->timer<0) {
        animator->timer += animator->current_animation->delay_ms;
        uint8_t next_tile = sprite->tile_idx+1;
        if (next_tile > animator->current_animation->end_tile){
            next_tile = animator->current_animation->start_tile;
        }
        gfx_sprite_set_tileid(sprite,next_tile);
    }
}

