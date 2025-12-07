#ifndef __MH_GLOBALS_H_
#define __MH_GLOBALS_H_

#define INIT_MOORHUHN_AMOUNT 10
#define MAX_MOORHUHN_AMOUNT 10

#define PIXEL_SIZE 2
#define VIEW_MAX_X 150
#define VIEW_MOVE_SPEED 2

#define TICK_RATE (1000/30)
#define SPRITEAMOUNT MAX_MOORHUHN_AMOUNT+1


#ifdef _MOD_NATIVE_
# include "../../source/core/memory.h"
# include "../../source/ng_all.h"
#else
# include <ng_config.h>
# include "../../source/ng_all.h"
#endif

#define KEY_MOVE_VIEW_LEFT  (1 << 0)
#define KEY_MOVE_VIEW_RIGHT (1 << 1)

#define SINE_TABLE_SIZE 160
#define val_to_sin_idx(val) ((val) % SINE_TABLE_SIZE)

extern const int8_t sine_table[SINE_TABLE_SIZE];

typedef struct mh_renderstate_t {
    uint8_t spritebuffer_id;
    uint8_t tick_counter;
    int16_t view_x;

    gfx_tilesheet_data_t ts_bird;
    gfx_tilesheet_data_t ts_crosshair_16;
    gfx_tilesheet_data_t ts_background;

    gfx_sprite_t sprites[SPRITEAMOUNT];
    gfx_sprite_animator_t sprite_animators[MAX_MOORHUHN_AMOUNT];
    gfx_sprite_t* crosshair;
} mh_renderstate_t;

// created in mod_init.c
extern mh_renderstate_t mh_rs; 
extern gfx_sprite_animator_t anim4x1;
extern gfx_sprite_animator_t anim4x1_60;

#endif